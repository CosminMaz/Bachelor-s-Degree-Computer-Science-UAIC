"""
Database module for the Resource Monitor.

Handles SQLite database initialization and data logging for resource metrics.
"""
import sqlite3
import time

DB_FILE = "resource_monitor.db"

def init_db(db_file=DB_FILE):
    """Initialize the SQLite database and create tables if they don't exist."""
    conn = sqlite3.connect(db_file)
    c = conn.cursor()

    # CPU History Table
    c.execute('''
        CREATE TABLE IF NOT EXISTS cpu_history (
            timestamp REAL PRIMARY KEY,
            cpu_percent REAL
        )
    ''')

    # Memory History Table
    c.execute('''
        CREATE TABLE IF NOT EXISTS memory_history (
            timestamp REAL PRIMARY KEY,
            mem_percent REAL
        )
    ''')

    # Disk I/O History Table
    c.execute('''
        CREATE TABLE IF NOT EXISTS disk_io_history (
            timestamp REAL PRIMARY KEY,
            read_bytes INTEGER,
            write_bytes INTEGER
        )
    ''')

    # Network I/O History Table
    c.execute('''
        CREATE TABLE IF NOT EXISTS network_io_history (
            timestamp REAL PRIMARY KEY,
            bytes_sent INTEGER,
            bytes_recv INTEGER
        )
    ''')

    conn.commit()
    conn.close()

def log_data(table, data, db_file=DB_FILE):
    """
    Insert or replace a row in the specified table.

    Args:
        table (str): The table name.
        data (dict): A dictionary mapping column names to values.
        db_file (str): The path to the database file.
    """
    conn = sqlite3.connect(db_file)
    c = conn.cursor()
    
    placeholders = ', '.join(['?'] * len(data))
    columns = ', '.join(data.keys())
    
    sql = f"INSERT OR REPLACE INTO {table} ({columns}) VALUES ({placeholders})"
    
    try:
        c.execute(sql, tuple(data.values()))
        conn.commit()
    except sqlite3.IntegrityError:
        # This can happen if two updates try to write the same timestamp.
        pass
    finally:
        conn.close()

def log_cpu(data, db_file=DB_FILE):
    """Log CPU data to the database."""
    log_data("cpu_history", {
        "timestamp": time.time(),
        "cpu_percent": data["cpu_percent"]
    }, db_file)

def log_memory(data, db_file=DB_FILE):
    """Log Memory data to the database."""
    log_data("memory_history", {
        "timestamp": time.time(),
        "mem_percent": data["mem"].percent
    }, db_file)

def log_disk(data, db_file=DB_FILE):
    """Log Disk I/O data to the database."""
    log_data("disk_io_history", {
        "timestamp": time.time(),
        "read_bytes": data["read_bytes_total"],
        "write_bytes": data["write_bytes_total"]
    }, db_file)

def log_network(data, db_file=DB_FILE):
    """Log Network I/O data to the database."""
    log_data("network_io_history", {
        "timestamp": time.time(),
        "bytes_sent": data["sent_bytes"],
        "bytes_recv": data["recv_bytes"]
    }, db_file)
