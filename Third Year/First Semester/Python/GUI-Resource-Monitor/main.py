#!/usr/bin/python3
"""
Main entry point for the GUI Resource Monitor application.

This script initializes and runs the main application window.
"""
from gui_resource_monitor.resource_monitor import ResourceMonitor

if __name__ == "__main__":
    app = ResourceMonitor()
    app.mainloop()