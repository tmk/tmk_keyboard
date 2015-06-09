﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Hid;

namespace Project1HostApp
{
    public partial class frmDataloggerSettings : Form
    {
        private const int DEVICE_VID = 0x03EB;
        private const int DEVICE_PID = 0x2063;

        private struct Device_Report_t
        {
            public Byte Day;
            public Byte Month;
            public Byte Year;

            public Byte Hour;
            public Byte Minute;
            public Byte Second;

            public Byte LogInterval500MS;

            public Byte[] ToReport()
            {
                Byte[] Report = new Byte[7];

                Report[0] = this.Hour;
                Report[1] = this.Minute;
                Report[2] = this.Second;
                Report[3] = this.Day;
                Report[4] = this.Month;
                Report[5] = this.Year;
                Report[6] = this.LogInterval500MS;

                return Report;
            }

            public void FromReport(Byte[] Report)
            {
                this.Hour = Report[0];
                this.Minute = Report[1];
                this.Second = Report[2];
                this.Day = Report[3];
                this.Month = Report[4];
                this.Year = Report[5];
                this.LogInterval500MS = Report[6];
            }
        };

        private IDevice GetDeviceConnection()
        {
            IDevice[] ConnectedDevices = DeviceFactory.Enumerate(DEVICE_VID, DEVICE_PID);
            IDevice ConnectionHandle = null;

            if (ConnectedDevices.Count() > 0)
                ConnectionHandle = ConnectedDevices[0];
            else
                return null;

            // Fix report handle under Windows
            if (ConnectionHandle is Hid.Win32.Win32DeviceSet)
            {
                ((Hid.Win32.Win32DeviceSet)ConnectionHandle).AddDevice(0x00,
                    ((Hid.Win32.Win32DeviceSet)ConnectionHandle).UnallocatedDevices[0]);
            }

            return ConnectionHandle;
        }

        public frmDataloggerSettings()
        {
            InitializeComponent();
        }

        private void btnSetValues_Click(object sender, EventArgs e)
        {
            IDevice ConnectionHandle = GetDeviceConnection();

            if (ConnectionHandle == null)
            {
                MessageBox.Show("Error: Cannot connect to Datalogger device.");
                return;
            }

            Device_Report_t DeviceReport = new Device_Report_t();
            DeviceReport.Day = (byte)dtpDate.Value.Day;
            DeviceReport.Month = (byte)dtpDate.Value.Month;
            DeviceReport.Year = (byte)((dtpDate.Value.Year < 2000) ? 0 : (dtpDate.Value.Year - 2000));
            DeviceReport.Hour = (byte)dtpTime.Value.Hour;
            DeviceReport.Minute = (byte)dtpTime.Value.Minute;
            DeviceReport.Second = (byte)dtpTime.Value.Second;
            DeviceReport.LogInterval500MS = (byte)(nudLogInterval.Value * 2);

            try
            {
                ConnectionHandle.Write(0x00, DeviceReport.ToReport());
                MessageBox.Show("Device parameters updated successfully.");
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
        }

        private void btnGetValues_Click(object sender, EventArgs e)
        {
            IDevice ConnectionHandle = GetDeviceConnection();

            if (ConnectionHandle == null)
            {
                MessageBox.Show("Error: Cannot connect to Datalogger device.");
                return;
            }

            Device_Report_t DeviceReport = new Device_Report_t();

            try
            {
                Byte[] Report = new Byte[7];

                ConnectionHandle.Read(0x00, Report);
                DeviceReport.FromReport(Report);
                String msgText = "Device parameters retrieved successfully.";

                try
                {
                    dtpDate.Value = new DateTime(
                        (2000 + DeviceReport.Year),
                        DeviceReport.Month,
                        DeviceReport.Day);

                    dtpTime.Value = new DateTime(
                        DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day,
                        DeviceReport.Hour,
                        DeviceReport.Minute,
                        DeviceReport.Second);
                }
                catch (Exception ex)
                {
                    msgText = "Problem reading device:\n" +
                        ex.Message +
                        "\nY:" + DeviceReport.Year.ToString() +
                        " M:" + DeviceReport.Month.ToString() +
                        " D:" + DeviceReport.Day.ToString() +
                        "\n\nUsing current date and time.";
                    dtpDate.Value = DateTime.Now;
                    dtpTime.Value = DateTime.Now;
                }

                try
                {
                    nudLogInterval.Value = (DeviceReport.LogInterval500MS / 2);
                }
                catch (Exception ex)
                {
                    nudLogInterval.Value = nudLogInterval.Minimum;
                }

                MessageBox.Show(msgText);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.Message);
            }
        }

        private void frmDataloggerSettings_Load(object sender, EventArgs e)
        {

        }
    }
}
