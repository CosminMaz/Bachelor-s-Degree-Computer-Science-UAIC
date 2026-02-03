import unittest
from unittest.mock import patch, mock_open

# Add project root to path to allow imports
import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from gui_resource_monitor.utils import get_size, get_drive_type

class TestUtils(unittest.TestCase):
    """Tests for the utility functions in utils.py"""

    def test_get_size(self):
        self.assertEqual(get_size(1023), "1023.00B")
        self.assertEqual(get_size(1024), "1.00KB")
        self.assertEqual(get_size(1536), "1.50KB")
        self.assertEqual(get_size(1024 * 1024 * 5.25), "5.25MB")
        self.assertEqual(get_size(1024 * 1024 * 1024 * 2), "2.00GB")

    @patch("os.path.exists")
    @patch("builtins.open", new_callable=mock_open)
    def test_get_drive_type_ssd(self, mock_file, mock_exists):
        """Test get_drive_type for SSD."""
        def exists_side_effect(path):
            # It's a partition, so pkname exists. Rotational file also exists for parent.
            return path in ('/sys/class/block/sda1/pkname', '/sys/class/block/sda/queue/rotational')
        mock_exists.side_effect = exists_side_effect

        # open is called twice. First for pkname, second for rotational.
        mock_file.return_value.read.side_effect = ["sda\n", "0\n"]

        self.assertEqual(get_drive_type("sda1"), "SSD")

        # Check calls to open
        mock_file.assert_any_call('/sys/class/block/sda1/pkname', 'r')
        mock_file.assert_any_call('/sys/class/block/sda/queue/rotational', 'r')

    @patch("os.path.exists")
    @patch("builtins.open", new_callable=mock_open, read_data="1\n")
    def test_get_drive_type_hdd(self, mock_file, mock_exists):
        """Test get_drive_type for HDD."""
        def exists_side_effect(path):
            # For a disk 'sdb', pkname should not exist, but rotational should.
            if path == '/sys/class/block/sdb/pkname':
                return False
            return path == '/sys/class/block/sdb/queue/rotational'
        mock_exists.side_effect = exists_side_effect

        self.assertEqual(get_drive_type("sdb"), "HDD")
        mock_file.assert_called_with('/sys/class/block/sdb/queue/rotational', 'r')

    @patch("os.path.exists")
    def test_get_drive_type_nvme(self, mock_exists):
        """Test get_drive_type for NVMe drives by name."""
        mock_exists.return_value = False # Don't need to find rotational file
        self.assertEqual(get_drive_type("nvme0n1p1"), "SSD")

    @patch("os.path.exists")
    def test_get_drive_type_na(self, mock_exists):
        """Test get_drive_type for devices where type is not applicable."""
        # Mock the rotational file not existing
        mock_exists.return_value = False
        self.assertEqual(get_drive_type("loop0"), "N/A")