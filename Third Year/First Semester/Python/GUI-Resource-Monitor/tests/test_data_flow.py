import unittest
from unittest.mock import patch, MagicMock, ANY
import tkinter as tk
import os
import time
import sqlite3
import collections
from collections import namedtuple
import queue

# Add project root to path to allow imports
import sys
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

# Import modules to be tested
from gui_resource_monitor.cpu_tab import CPUTab
from gui_resource_monitor.memory_tab import MemoryTab
from gui_resource_monitor.storage_tab import StorageTab
from gui_resource_monitor.network_tab import NetworkTab
from gui_resource_monitor.history_tab import HistoryTab
from gui_resource_monitor.resource_monitor import DataUpdateThread
from gui_resource_monitor import database

# --- Mock Data Structures ---
# These mimic the structures returned by psutil
sdiskpart = namedtuple('sdiskpart', ['device', 'mountpoint', 'fstype', 'opts'])
sdiskusage = namedtuple('sdiskusage', ['total', 'used', 'free', 'percent'])
sdiskio = namedtuple('sdiskio', ['read_count', 'write_count', 'read_bytes', 'write_bytes', 'read_time', 'write_time'])
svmem = namedtuple('svmem', ['total', 'available', 'percent', 'used', 'free'])
sswap = namedtuple('sswap', ['total', 'used', 'free', 'percent', 'sin', 'sout'])
snetio = namedtuple('snetio', ['bytes_sent', 'bytes_recv', 'packets_sent', 'packets_recv', 'errin', 'errout', 'dropin', 'dropout'])
snicaddr = namedtuple('snicaddr', ['family', 'address', 'netmask', 'broadcast', 'ptp'])
snicstats = namedtuple('snicstats', ['isup', 'duplex', 'speed', 'mtu'])
scpufreq = namedtuple('scpufreq', ['current', 'min', 'max'])

TEST_DB = "test_monitor.db"

class TestDatabase(unittest.TestCase):
    """Tests for the database module."""

    def setUp(self):
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)
        database.init_db(db_file=TEST_DB)
        self.conn = sqlite3.connect(TEST_DB)

    def tearDown(self):
        self.conn.close()
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)

    def test_log_cpu(self):
        cpu_data = {"cpu_percent": 55.5}
        database.log_cpu(cpu_data, db_file=TEST_DB)
        
        cursor = self.conn.cursor()
        cursor.execute("SELECT cpu_percent FROM cpu_history")
        result = cursor.fetchone()
        self.assertIsNotNone(result)
        self.assertAlmostEqual(result[0], 55.5)

    def test_log_memory(self):
        mem_data = {"mem": svmem(total=0, available=0, percent=75.2, used=0, free=0)}
        database.log_memory(mem_data, db_file=TEST_DB)
        
        cursor = self.conn.cursor()
        cursor.execute("SELECT mem_percent FROM memory_history")
        result = cursor.fetchone()
        self.assertIsNotNone(result)
        self.assertAlmostEqual(result[0], 75.2)

    def test_log_disk(self):
        disk_data = {"read_bytes_total": 1024, "write_bytes_total": 2048}
        database.log_disk(disk_data, db_file=TEST_DB)
        
        cursor = self.conn.cursor()
        cursor.execute("SELECT read_bytes, write_bytes FROM disk_io_history")
        result = cursor.fetchone()
        self.assertIsNotNone(result)
        self.assertEqual(result[0], 1024)
        self.assertEqual(result[1], 2048)

    def test_log_network(self):
        net_data = {"sent_bytes": 512, "recv_bytes": 1024}
        database.log_network(net_data, db_file=TEST_DB)
        
        cursor = self.conn.cursor()
        cursor.execute("SELECT bytes_sent, bytes_recv FROM network_io_history")
        result = cursor.fetchone()
        self.assertIsNotNone(result)
        self.assertEqual(result[0], 512)
        self.assertEqual(result[1], 1024)


class TestDataCollection(unittest.TestCase):
    """Base class for data collection tests, sets up a root Tk window."""
    def setUp(self):
        self.root = tk.Tk()
        self.root.withdraw() # Hide the window

    def tearDown(self):
        self.root.destroy()

@patch('gui_resource_monitor.cpu_tab.psutil')
@patch('gui_resource_monitor.cpu_tab.cpuinfo')
class TestCPUData(TestDataCollection):
    """Tests for CPU data collection."""

    def test_fetch_cpu_data(self, mock_cpuinfo, mock_psutil):
        # The call in fetch_data will return this list.
        mock_psutil.cpu_percent.return_value = [10.0, 20.0, 30.0, 40.0]
        mock_psutil.cpu_freq.return_value = scpufreq(current=2500.0, min=800.0, max=3500.0)
        mock_psutil.sensors_temperatures.return_value = {'coretemp': [MagicMock(label='Core 0', current=60.0)]}
        mock_cpuinfo.get_cpu_info.return_value = {'brand_raw': 'Test CPU'}

        cpu_tab = CPUTab(self.root)
        data = cpu_tab.fetch_data()

        self.assertAlmostEqual(data['cpu_percent'], 25.0)
        self.assertEqual(data['per_cpu'], [10.0, 20.0, 30.0, 40.0])
        self.assertEqual(data['cpu_freq'].current, 2500.0)
        self.assertIn("Core 0: 60.0°C", data['temp_str'])

    def test_fetch_cpu_data_no_freq(self, mock_cpuinfo, mock_psutil):
        """Test CPU data fetching when cpu_freq throws an exception."""
        mock_psutil.cpu_percent.return_value = [10.0, 20.0]
        # Simulate psutil.cpu_freq() failing
        mock_psutil.cpu_freq.side_effect = Exception("Cannot read frequency")
        mock_psutil.sensors_temperatures.return_value = {}
        mock_cpuinfo.get_cpu_info.return_value = {'brand_raw': 'Test CPU'}

        cpu_tab = CPUTab(self.root)
        data = cpu_tab.fetch_data()

        self.assertAlmostEqual(data['cpu_percent'], 15.0)
        # Verify that cpu_freq is None when the call fails, ensuring graceful failure
        self.assertIsNone(data['cpu_freq'])

@patch('gui_resource_monitor.memory_tab.psutil')
class TestMemoryData(TestDataCollection):
    """Tests for Memory data collection."""

    def test_fetch_memory_data(self, mock_psutil):
        mock_psutil.virtual_memory.return_value = svmem(total=8e9, available=4e9, percent=50.0, used=4e9, free=4e9)
        mock_psutil.swap_memory.return_value = sswap(total=2e9, used=1e9, free=1e9, percent=50.0, sin=0, sout=0)

        mem_tab = MemoryTab(self.root)
        data = mem_tab.fetch_data()

        self.assertEqual(data['mem'].percent, 50.0)
        self.assertEqual(data['swap'].percent, 50.0)

@patch('time.time')
@patch('gui_resource_monitor.storage_tab.get_drive_type', return_value="SSD")
@patch('gui_resource_monitor.storage_tab.psutil')
class TestStorageData(TestDataCollection):
    """Tests for Storage data collection."""

    def test_fetch_storage_data(self, mock_psutil, mock_get_drive_type, mock_time):
        mock_psutil.disk_partitions.return_value = [sdiskpart(device='/dev/sda1', mountpoint='/', fstype='ext4', opts='rw')]
        mock_psutil.disk_usage.return_value = sdiskusage(total=100, used=50, free=50, percent=50.0)
        initial_io = {'sda1': sdiskio(read_bytes=1000, write_bytes=2000, read_count=0, write_count=0, read_time=0, write_time=0)}
        final_io = {'sda1': sdiskio(read_bytes=2024, write_bytes=4048, read_count=0, write_count=0, read_time=0, write_time=0)}
        mock_psutil.disk_io_counters.side_effect = [initial_io, final_io]
        mock_time.side_effect = [0.0, 1.0]

        storage_tab = StorageTab(self.root)
        data = storage_tab.fetch_data()

        self.assertEqual(len(data['partition_data']), 1)
        self.assertAlmostEqual(data['read_bytes_total'], 1024.0)
        self.assertAlmostEqual(data['write_bytes_total'], 2048.0)

@patch('time.time')
@patch('gui_resource_monitor.network_tab.psutil')
class TestNetworkData(TestDataCollection):
    """Tests for Network data collection."""

    def test_fetch_network_data(self, mock_psutil, mock_time):
        initial_io = snetio(bytes_sent=1000, bytes_recv=2000, packets_sent=0, packets_recv=0, errin=0, errout=0, dropin=0, dropout=0)
        final_io = snetio(bytes_sent=1512, bytes_recv=3024, packets_sent=0, packets_recv=0, errin=0, errout=0, dropin=0, dropout=0)
        mock_psutil.net_io_counters.side_effect = [initial_io, final_io]
        mock_psutil.net_if_addrs.return_value = {'eth0': [snicaddr(family=2, address='192.168.1.100', netmask='255.255.255.0', broadcast=None, ptp=None)]}
        mock_psutil.net_if_stats.return_value = {'eth0': snicstats(isup=True, duplex=0, speed=1000, mtu=1500)}
        mock_time.side_effect = [0.0, 1.0]

        net_tab = NetworkTab(self.root)
        data = net_tab.fetch_data()

        self.assertAlmostEqual(data['sent_bytes'], 512.0)
        self.assertAlmostEqual(data['recv_bytes'], 1024.0)
        self.assertEqual(data['interface_data'][0]['ip'], '192.168.1.100')

class TestHistoryData(TestDataCollection):
    """Tests for the History tab's data fetching."""

    def setUp(self):
        super().setUp()
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)
        database.init_db(db_file=TEST_DB)
        database.log_cpu({"cpu_percent": 50.0}, db_file=TEST_DB)

        self.history_tab = HistoryTab(self.root)
        self.history_tab.db_file = TEST_DB

    def tearDown(self):
        super().tearDown()
        if os.path.exists(TEST_DB):
            os.remove(TEST_DB)

    def test_fetch_history(self):
        timestamps, values = self.history_tab.fetch_history("cpu_history", "timestamp", ["cpu_percent"])
        self.assertEqual(len(timestamps), 1)
        self.assertAlmostEqual(values[0][0], 50.0)

@patch('gui_resource_monitor.resource_monitor.database')
class TestDataUpdateThread(unittest.TestCase):
    """Integration test for the DataUpdateThread."""

    @patch('time.sleep')
    def test_thread_flow(self, mock_sleep, mock_database):
        mock_cpu_tab = MagicMock()
        mock_cpu_tab.fetch_data.return_value = {"cpu_percent": 50}
        tabs = {"cpu": mock_cpu_tab}

        mock_app = MagicMock()
        q = queue.Queue()
        mock_app.queue = q

        thread = DataUpdateThread(mock_app, tabs, update_interval=0.01)
        thread.start()
        time.sleep(0.1) # Allow thread to run
        thread.stop()
        thread.join()

        mock_cpu_tab.fetch_data.assert_called()
        self.assertFalse(q.empty())
        queued_data = q.get_nowait()
        self.assertEqual(queued_data["cpu"]["cpu_percent"], 50)
        mock_database.log_cpu.assert_called_with({"cpu_percent": 50})


if __name__ == '__main__':
    unittest.main()