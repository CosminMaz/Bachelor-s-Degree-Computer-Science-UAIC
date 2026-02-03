"""
The main application class for the GUI Resource Monitor.
"""
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import queue
import time
import json
import os
from .cpu_tab import CPUTab
from .memory_tab import MemoryTab
from .storage_tab import StorageTab
from .network_tab import NetworkTab
from .history_tab import HistoryTab
from . import database

class DataUpdateThread(threading.Thread):
    """
    A background thread that fetches data from all tabs and updates the UI queue.
    """

    def __init__(self, app, tabs, update_interval=1):
        """
        Initialize the update thread.

        Args:
            app (ResourceMonitor): The main application instance.
            tabs (dict): Dictionary of tab instances.
            update_interval (int): Time in seconds between updates.
        """
        super().__init__(daemon=True)
        self.app = app
        self.tabs = tabs
        self.update_interval = update_interval
        self.stopped = threading.Event()

        # Map tab names to their database logging functions for cleaner code
        self.log_functions = {
            "cpu": database.log_cpu,
            "memory": database.log_memory,
            "storage": database.log_disk,
            "network": database.log_network,
        }
    def run(self):
        """Main loop for fetching data, queuing it for the UI, and logging it to the database."""
        while not self.stopped.is_set():
            data = {}
            # Fetch all data, with error handling for each tab
            for name, tab in self.tabs.items():
                if hasattr(tab, 'fetch_data'):
                    try:
                        data[name] = tab.fetch_data()
                    except Exception as e:
                        print(f"Error fetching data for {name}: {e}")

            # Queue data for UI update as soon as it's ready
            if data:
                self.app.queue.put(data)

            # Log all successfully fetched data to the database
            for name, fetched_data in data.items():
                if name in self.log_functions:
                    try:
                        self.log_functions[name](fetched_data)
                    except Exception as e:
                        print(f"Error logging data for {name}: {e}")

            time.sleep(self.update_interval)

    def stop(self):
        """Signal the thread to stop."""
        self.stopped.set()

class ResourceMonitor(tk.Tk):
    """The main application class for the Resource Monitor."""

    def __init__(self):
        """Initialize the main window and all tabs."""
        super().__init__()
        self.title("GUI Resource Monitor")
        self.geometry("1000x700")
        self.minsize(600, 500)

        self.themes = self.load_themes()
        self.current_theme = "light"  # Default theme

        database.init_db()

        self.create_menu()

        self.protocol("WM_DELETE_WINDOW", self.on_close)

        self.notebook = ttk.Notebook(self)
        self.notebook.pack(pady=10, padx=10, expand=True, fill="both")

        self.cpu_tab = CPUTab(self.notebook)
        self.notebook.add(self.cpu_tab, text="CPU")
        
        self.memory_tab = MemoryTab(self.notebook)
        self.notebook.add(self.memory_tab, text="Memory")
        
        self.storage_tab = StorageTab(self.notebook)
        self.notebook.add(self.storage_tab, text="Storage")

        self.network_tab = NetworkTab(self.notebook)
        self.notebook.add(self.network_tab, text="Network")

        self.history_tab = HistoryTab(self.notebook)
        self.notebook.add(self.history_tab, text="History")

        self.notebook.bind("<<NotebookTabChanged>>", self.on_tab_changed)

        self.tabs = {
            "cpu": self.cpu_tab,
            "memory": self.memory_tab,
            "storage": self.storage_tab,
            "network": self.network_tab,
        }

        self.queue = queue.Queue()
        self.update_thread = DataUpdateThread(self, self.tabs)
        self.update_thread.start()

        self.update_ui()
        self.apply_theme()

    def load_themes(self):
        """Load theme configurations from a JSON file."""
        try:
            project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
            theme_file_path = os.path.join(project_root, "themes.json")
            with open(theme_file_path, 'r') as f:
                return json.load(f)
        except (FileNotFoundError, json.JSONDecodeError) as e:
            messagebox.showwarning("Theme File Error", f"Could not load themes.json: {e}\nUsing default themes.")
            # Fallback to hardcoded themes
            return {
                "light": {
                    "name": "Light", "type": "light", 
                    "colors": {
                        'bg': '#f0f0f0', 'fg': 'black', 'select_bg': '#dcdcdc',
                        'notebook_tab_bg': '#e0e0e0', 'treeview_bg': 'white', 'entry_field_bg': 'white',
                        'button_bg': '#e0e0e0', 'button_active_bg': '#ebebeb', 'button_pressed_bg': '#dcdcdc',
                        'checkbutton_indicator_selected': '#dcdcdc', 'scrollbar_bg': '#e0e0e0',
                        'scrollbar_trough_bg': '#f0f0f0', 'scrollbar_active_bg': '#dcdcdc',
                        'progressbar': {
                            'background': '#2196f3',
                            'troughcolor': '#e0e0e0',
                            'bordercolor': '#e0e0e0'
                        }
                    }
                }
            }
    def create_menu(self):
        """Create the application menu bar."""
        menubar = tk.Menu(self)
        menubar.add_command(label="Settings", command=self.show_settings)
        menubar.add_command(label="Exit", command=self.on_close)
        self.config(menu=menubar)

    def show_settings(self):
        """Open the settings dialog."""
        settings_window = tk.Toplevel(self)
        settings_window.title("Settings")
        settings_window.minsize(300, 250)

        theme_colors = self.themes.get(self.current_theme, {}).get('colors', {})
        if 'bg' in theme_colors:
            settings_window.configure(bg=theme_colors['bg'])
        settings_window.transient(self)
        
        container = ttk.Frame(settings_window, padding=20)
        container.pack(fill='both', expand=True)
        
        # --- Update Interval ---
        interval_frame = ttk.LabelFrame(container, text="Performance")
        interval_frame.pack(fill='x', pady=(0, 10))

        ttk.Label(interval_frame, text="Update Interval (seconds):").pack(anchor='w', padx=10, pady=(5, 0))

        interval_var = tk.StringVar(value=str(self.update_thread.update_interval))
        entry = ttk.Entry(interval_frame, textvariable=interval_var)
        entry.pack(fill='x', padx=10, pady=(0, 10))
        
        # --- Theme Selection ---
        theme_frame = ttk.LabelFrame(container, text="Appearance")
        theme_frame.pack(fill='x', pady=(0, 15))

        theme_var = tk.StringVar(value=self.current_theme)
        
        for theme_id, theme_data in self.themes.items():
            ttk.Radiobutton(theme_frame, text=theme_data.get("name", theme_id), variable=theme_var, value=theme_id).pack(anchor='w', padx=10, pady=(2, 2))
        
        def save_settings():
            try:
                new_interval = float(interval_var.get())
                if new_interval <= 0:
                    raise ValueError("Interval must be positive")
                self.update_thread.update_interval = new_interval
                
                new_theme = theme_var.get()
                if new_theme != self.current_theme:
                    self.current_theme = new_theme
                    self.apply_theme()
                
                settings_window.destroy()
            except ValueError:
                messagebox.showerror("Invalid Input", "Please enter a valid positive number.", parent=settings_window)

        ttk.Button(container, text="Save", command=save_settings).pack(fill='x')

    def apply_theme(self):
        """Apply the selected theme (light or dark) to the application."""
        style = ttk.Style(self)

        # Use 'clam' theme for both modes for consistent styling and layout capabilities.
        style.theme_use('clam')

        # Redefine the TNotebook.Tab layout to make tabs expand and fill the width.
        # The 'sticky': 'nswe' on 'Notebook.tab' is the key part that makes it expand.
        try:
            # Define the layout for a single tab. This structure is nested
            # to control how elements like padding and focus are drawn.
            # By breaking it into a separate, formatted variable, it's easier to read.
            tab_layout = [
                ('Notebook.tab', {'sticky': 'nswe', 'children': [
                    ('Notebook.padding', {'side': 'top', 'sticky': 'nswe', 'children': [
                        ('Notebook.focus', {'side': 'top', 'sticky': 'nswe', 'children': [
                            ('Notebook.label', {'side': 'top', 'sticky': ''})
                        ]})
                    ]})
                ]})
            ]
            style.layout("TNotebook.Tab", tab_layout)
        except tk.TclError:
            print("Warning: Could not apply expanding tabs style.")

        theme_data = self.themes.get(self.current_theme)
        if not theme_data:
            messagebox.showerror("Theme Error", f"Theme '{self.current_theme}' not found. Reverting to light theme.")
            self.current_theme = "light"
            theme_data = self.themes.get("light")

        colors = theme_data['colors']
        mode = theme_data.get('type', 'light')

        # Apply common styles using the selected color palette
        style.configure('.', background=colors['bg'], foreground=colors['fg'])
        style.configure('TNotebook', background=colors['bg'], tabmargins=[2, 5, 2, 0])
        style.configure('TNotebook.Tab', background=colors['notebook_tab_bg'], foreground=colors['fg'], padding=[10, 2])
        style.map('TNotebook.Tab', background=[('selected', colors['select_bg'])], foreground=[('selected', colors['fg'])])
        style.configure('TLabelFrame', background=colors['bg'], bordercolor=colors['fg'])
        style.configure('TLabelFrame.Label', background=colors['bg'], foreground=colors['fg'])
        
        style.configure('Treeview', background=colors['treeview_bg'], foreground=colors['fg'], fieldbackground=colors['treeview_bg'])
        style.map('Treeview', background=[('selected', colors['select_bg'])], foreground=[('selected', colors['fg'])])
        
        style.configure('TEntry', fieldbackground=colors['entry_field_bg'], foreground=colors['fg'])
        
        style.configure('TButton', background=colors['button_bg'], foreground=colors['fg'], borderwidth=1)
        style.map('TButton',
                  background=[('active', colors['button_active_bg']), ('pressed', colors['button_pressed_bg'])],
                  foreground=[('disabled', '#a3a3a3')])
        
        style.configure('TCheckbutton', background=colors['bg'], foreground=colors['fg'])
        style.map('TCheckbutton', background=[('active', colors['bg'])], indicatorcolor=[('selected', colors['checkbutton_indicator_selected'])])
        
        style.configure('TScrollbar', background=colors['scrollbar_bg'], troughcolor=colors['scrollbar_trough_bg'], bordercolor=colors['scrollbar_trough_bg'], arrowcolor=colors['fg'])
        style.map('TScrollbar', background=[('active', colors['scrollbar_active_bg'])])
        self.configure(bg=colors['bg'])

        for tab in [self.cpu_tab, self.memory_tab, self.storage_tab, self.network_tab, self.history_tab]:
            if hasattr(tab, 'set_theme'):
                tab.set_theme(theme_data)

    def on_tab_changed(self, event):
        """Handle tab change events to trigger specific updates"""
        selected_tab = self.notebook.nametowidget(self.notebook.select())

        # When a tab is selected, its contents might not have the correct size
        # if the window was resized while the tab was hidden.
        # Forcing a <Configure> event on the tab's frame will trigger
        # the resize logic within the tab to correctly adjust its layout.
        selected_tab.event_generate("<Configure>")

        if selected_tab == self.history_tab:
            self.history_tab.update_charts()

    def update_ui(self):
        """Process the data queue and update the UI of the active tabs."""
        data = None
        try:
            # Drain the queue to get the latest data and avoid backlog
            while True:
                data = self.queue.get_nowait()
        except queue.Empty:
            pass
        
        if data:
            if "cpu" in data and hasattr(self.cpu_tab, 'update_ui'):
                self.cpu_tab.update_ui(data["cpu"])
            if "memory" in data and hasattr(self.memory_tab, 'update_ui'):
                self.memory_tab.update_ui(data["memory"])
            if "storage" in data and hasattr(self.storage_tab, 'update_ui'):
                self.storage_tab.update_ui(data["storage"])
            if "network" in data and hasattr(self.network_tab, 'update_ui'):
                self.network_tab.update_ui(data["network"])

        self.after(100, self.update_ui)

    def on_close(self):
        """Handle application closure, ensuring threads are stopped."""
        self.update_thread.stop()
        self.update_thread.join()
        self.destroy()