/*
 *  Atmel DFU WebUSB
 */
// AVR MEGA: https://ww1.microchip.com/downloads/en/DeviceDoc/doc7618.pdf
// AVR XMEGA: https://ww1.microchip.com/downloads/en/devicedoc/doc8457.pdf

// bRequest
// DFU_DETACH, DFU_GETSTATE, DFU_ABORT is not defined on XMEGA and not used in dfu-programmer
export const bRequest = {
    DFU_DETACH: 0,
    DFU_DNLOAD: 1,
    DFU_UPLOAD: 2,
    DFU_GETSTATUS: 3,
    DFU_CLRSTATUS: 4,
    DFU_GETSTATE: 5,
    DFU_ABORT: 6,
};

// bStatus
export const bStatus = {
    OK: 0,
    errTARGET: 1,
    errFILE: 2,
    errWRITE: 3,
    errERASE: 4,
    errCHECK_ERASED: 5,
    errPROG: 6,
    errVERIFY: 7,
    errADDRESS: 8,
    errNOTDONE: 9,
    errFIRMWARE: 10,
    errVENDOR: 11,
    errUSBR: 12,
    errPOR: 13,
    errUNKNOWN: 14,
    errSTALLEDPK: 15,
};

// bState
export const bState = {
    appIDLE: 0,
    appDETACH: 1,
    dfuIDLE: 2,
    dfuDNLOAD_SYNC: 3,
    dfuDNBUSY: 4,
    dfuDNLOAD_IDLE: 5,
    dfuMANIFEST_SYNC: 6,
    dfuMANIFEST: 7,
    dfuMANIFEST_WAIT_RESET: 8,
    dfuUPLOAD_IDLE: 9,
    dfuERROR: 10,
};

const TRANSFER_SIZE = 0x400;
const PAGE_SIZE = 0x10000;

export const deviceInfo = [
    { name: "at90usb128x", vendorId: 0x03eb, productId: 0x2FFB, flashSize: 0x20000, bootSize: 0x2000, eepromSize: 0x1000 },
    { name: "at90usb64x",  vendorId: 0x03eb, productId: 0x2FF9, flashSize: 0x10000, bootSize: 0x2000, eepromSize: 0x0800 },
    { name: "at90usb162",  vendorId: 0x03eb, productId: 0x2FFA, flashSize: 0x04000, bootSize: 0x1000, eepromSize: 0x0200 },
    { name: "at90usb82",   vendorId: 0x03eb, productId: 0x2FF7, flashSize: 0x02000, bootSize: 0x1000, eepromSize: 0x0200 },
    { name: "atmega32u6",  vendorId: 0x03eb, productId: 0x2FF2, flashSize: 0x08000, bootSize: 0x1000, eepromSize: 0x0400 },
    { name: "atmega32u4",  vendorId: 0x03eb, productId: 0x2FF4, flashSize: 0x08000, bootSize: 0x1000, eepromSize: 0x0400 },
    { name: "atmega32u2",  vendorId: 0x03eb, productId: 0x2FF0, flashSize: 0x08000, bootSize: 0x1000, eepromSize: 0x0400 },
    { name: "atmega16u4",  vendorId: 0x03eb, productId: 0x2FF3, flashSize: 0x04000, bootSize: 0x1000, eepromSize: 0x0200 },
    { name: "atmega16u2",  vendorId: 0x03eb, productId: 0x2FEF, flashSize: 0x04000, bootSize: 0x1000, eepromSize: 0x0200 },
    { name: "atmega8u2",   vendorId: 0x03eb, productId: 0x2FEE, flashSize: 0x02000, bootSize: 0x1000, eepromSize: 0x0200 },
];

export async function getAtmelDevice() {
    try {
	let dev = await navigator.usb.requestDevice({ filters: [{ vendorId: 0x03eb /* atmel */ }] });
	await dev.open();
	await dev.claimInterface(0);
	return dev;
    } catch (e) {
        console.log(e);
        return null;
    }
}

export function parseStatus(data) {
    return {
        bStatus:        data.getUint8(0),
        bwPollTimeOut:  data.getUint8(1)
                        | data.getUint8(2) << 8
                        | data.getUint8(3) << 16,
        bState:         data.getUint8(4),
        iString:        data.getUint8(5),
    };
}

export function getStatus(dev) {
    /* USBInTransferResult
     *    .data: GETSTATUS response(DataView)
     *              [0]:    bStatus
     *              [1..3]: bwPollTimeOut
     *              [4]:    bState
     *              [5]:    iString
     *    .status: 'ok', 'stall', or 'babble'
     */
    return dev.controlTransferIn(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_GETSTATUS,
            value:          0,
            index:          0,  // interface
        },
        6 /* length */
    );
};

export function selectPage(dev, page) {
    const cmd = new Uint8Array([ 0x06, 0x03, 0x00, 0x00 ]);
    cmd[3] = page;
    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        cmd
    );
}

export function writeBlock(dev, start, end, data, eeprom = false) {
    if (eeprom) {
        if (start % 8 !== 0) throw new Error(`Not page aligned: ${start}`)
    } else {
        if (start % 256 !== 0) throw new Error(`Not page aligned: ${start}`)
    }

    const header = [];
    header[0] = 0x01;   // 4.6.1.1 Write Command
    header[1] = (eeprom ? 0x01 : 0x00); // Flash:0  EEPROM:1
    header[2] = (start >> 8) & 0xff;
    header[3] = start & 0xff;
    header[4] = (end >> 8) & 0xff;
    header[5] = end & 0xff;
    header[31] = 0x00;  // bMaxPacketSize0 = 32

    //const footer = new Uint8Array(16);
    const footer = [];
    footer[0] = 0x00;   // CRC
    footer[1] = 0x00;
    footer[2] = 0x00;
    footer[3] = 0x00;
    footer[4] = 0x10;   // length of suffix
    footer[5] = 0x44;   // 'D'
    footer[6] = 0x46;   // 'F'
    footer[7] = 0x55;   // 'U'
    footer[8] = 0x01;   // bcdDFU
    footer[9] = 0x00;
    footer[10] = 0xff;  // idVendor
    footer[11] = 0xff;
    footer[12] = 0xff;  // idProduct
    footer[13] = 0xff;
    footer[14] = 0xff;  // bcdDevice
    footer[15] = 0xff;

    //let msg = header.concat(footer);
    let msg = new Uint8Array(header.length + data.length + footer.length);
    msg.set(header, 0);
    msg.set(data, header.length);
    msg.set(footer, header.length + data.length);

    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        msg
    );
}

export async function writeMemory(dev, start, end, data, eeprom = false) {
    if (start > end) throw new Error('Memory range error');

    let page = -1;
    let _start = start;
    let _end = 0;
    let result;
    let count = 0;

    while (_start < end) {
        if (page !== Math.floor(_start / PAGE_SIZE)) {
            page = Math.floor(_start / PAGE_SIZE);
            result = await selectPage(dev, page);
            if (result.status != 'ok') throw new Error('selectPage failure');
            result = await getStatus(dev);
            //console.log(`select page: ${ page }`);
        }

        _end = _start + TRANSFER_SIZE - 1;
        if (_end > end) _end = end;

        // avoid stepping over page border
        if (_end > (page + 1) * PAGE_SIZE) {
            _end = (page + 1) * PAGE_SIZE;
        }

        let d = data.slice(_start - start, _end - start + 1);
        //console.log(`write: page${page} ${hexStr(_start)} ... ${hexStr(_end)}`);
        result = await writeBlock(dev, _start % PAGE_SIZE, _end % PAGE_SIZE, d, eeprom);
        if (result.status != 'ok') throw new Error('writeBlock failure');
        //console.log('bytesWritten: ' + result.bytesWritten);
        //console.log('status: ' + result.status);

        count += result.bytesWritten;
        _start = _end + 1;
    }
    return count;
}

export async function readBlock(dev, start, end, eeprom = false) {
    if (dev === null) {
        return;
    }
    let cmd = new Uint8Array([ 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 ]);
    cmd[1] = (eeprom ? 0x02 : 0x00);
    cmd[2] = (start >> 8) & 0xff;
    cmd[3] = start & 0xff;
    cmd[4] = (end >> 8) & 0xff;
    cmd[5] = end & 0xff;

    let result = await dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        cmd
    );

    return dev.controlTransferIn(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_UPLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        (end - start + 1)
    );
};

export async function readMemory(dev, start, end, eeprom = false) {
    if (start > end) throw new Error('Memory range error');

    let page = -1;
    let _start = start;
    let _end = 0;
    let result;
    let buf = new Uint8Array(end - start + 1);

    while (_start < end) {
        if (page !== Math.floor(_start / PAGE_SIZE)) {
            page = Math.floor(_start / PAGE_SIZE);
            result = await selectPage(dev, page);
            result = await getStatus(dev);
            //console.log(`select page: ${ page }`);
        }

        _end = _start + TRANSFER_SIZE - 1;
        if (_end > end) _end = end;

        // avoid stepping over page border
        if (_end > (page + 1) * PAGE_SIZE) {
            _end = (page + 1) * PAGE_SIZE;
        }

        //console.log(`read: page${page} ${hexStr(_start)} ... ${hexStr(_end)}`);
        result = await readBlock(dev, _start % PAGE_SIZE, _end % PAGE_SIZE, eeprom);
        if (result.status != 'ok') {
            throw new Error(`readBlock failure at ${_start}`);
        }
        //console.log('byteLength: ' + result.data.byteLength);
        //console.log('status: ' + result.status);
        //console.log(result.data.buffer);

        buf.set(new Uint8Array(result.data.buffer), _start - start);
        _start = _end + 1;
    }
    return buf;
}

export function chipErase(dev) {
    const cmd = new Uint8Array([ 0x04, 0x00, 0xff ]);
    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        cmd
    );
}

export function blankCheck(dev, start, end) {
    const cmd = new Uint8Array([ 0x03, 0x01, 0x00, 0x00, 0x6f, 0xff ]);
    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        cmd
    );
}

export async function launch(dev) {
    const cmd = new Uint8Array([ 0x04, 0x03, 0x00 ]);
    await dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        },
        cmd
    );
    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_DNLOAD,
            value:          0,  // wBlock
            index:          0,  // interface
        }
    );
}

export function clearStatus(dev) {
    if (dev === null) {
        throw new Error('No device available');;
    }

    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_CLRSTATUS,
            value:          0,
            index:          0,  // interface
        },
    );
};

export function abort(dev) {
    if (dev === null) {
        return;
    }

    return dev.controlTransferOut(
        {
            requestType:    'class',
            recipient:      'interface',
            request:        bRequest.DFU_ABORT,
            value:          0,
            index:          0,  // interface
        },
    );
}

function hexStr(n, digit=2, toUpper=false) {
    return n.toString(16)
}
