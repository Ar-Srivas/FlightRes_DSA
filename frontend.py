import customtkinter
import tkinter as tk
import subprocess

# Set appearance mode and color theme
customtkinter.set_appearance_mode("dark")
customtkinter.set_default_color_theme("dark-blue")

# Create main application window
root = customtkinter.CTk()
root.geometry("850x650")

# Dictionary to keep track of open flight detail windows
open_details_windows = {}
# Dictionary to cache seat matrices for each flight
seat_matrices_cache = {}
# Dictionary to store reservations
reservations = {}

# Function to handle search and display results
def search_flights():
    query = search_entry.get().upper()  # Get user input from search bar
    print(f"Searching for flights from: {query}")  # Debugging statement
    results_frame.pack_forget()  # Remove previous buttons before new search results
    results_frame.pack(pady=12, padx=10, fill="both", expand=True)

    # Clear previous search results
    for widget in results_frame.winfo_children():
        widget.destroy()

    # Call the C++ program to get flight details
    result = subprocess.run(['./main_executable', 'flights', query], capture_output=True, text=True)

    # Check for errors
    if result.returncode != 0:
        print(f"Error fetching flights: {result.stderr}")  # Debugging statement
        return

    # Display the available flights returned by C++
    flight_details = result.stdout.strip().split('\n')
    for flight in flight_details:
        flight_button = customtkinter.CTkButton(master=results_frame, text=flight, command=lambda f=flight: open_flight_details(f))
        flight_button.pack(pady=5, padx=10)

# Function to open the flight details window
def open_flight_details(flight_name):
    if flight_name in open_details_windows:
        # If the window is already open, bring it to the front
        open_details_windows[flight_name].focus()
        return

    print(f"Opening details for flight: {flight_name}")  # Debugging statement
    details_window = customtkinter.CTkToplevel()
    details_window.geometry("800x600")
    details_window.title(f"Flight Details - {flight_name}")

    # Store the reference to the details window
    open_details_windows[flight_name] = details_window

    # Check if the seat matrix is cached; if not, fetch it
    seat_matrix_display = seat_matrices_cache.get(flight_name)
    if seat_matrix_display is None:
        result = subprocess.run(['./main_executable', 'seats'], capture_output=True, text=True)

        if result.returncode != 0 or "Invalid" in result.stdout:
            print(f"Error fetching seat matrix: {result.stderr or result.stdout}")  # Debugging statement
            return

        seat_matrix_output = result.stdout.strip().splitlines()
        seat_matrix_display = "\n".join(seat_matrix_output)

        # Cache the seat matrix for future reference
        seat_matrices_cache[flight_name] = seat_matrix_output

    # Define the seat matrix label here
    seat_matrix_label = customtkinter.CTkLabel(master=details_window, text="\n".join(seat_matrices_cache[flight_name]), font=("Roboto", 16))
    seat_matrix_label.pack(pady=20)

    booking_frame = customtkinter.CTkFrame(master=details_window)
    booking_frame.pack(side=tk.RIGHT, padx=20, pady=20, fill=tk.BOTH, expand=True)

    customer_name_label = customtkinter.CTkLabel(master=booking_frame, text="Customer Name:")
    customer_name_label.pack(pady=10)
    customer_name_entry = customtkinter.CTkEntry(master=booking_frame, placeholder_text="Enter customer name")
    customer_name_entry.pack(pady=10)

    seats_label = customtkinter.CTkLabel(master=booking_frame, text="Number of Seats:")
    seats_label.pack(pady=10)
    seats_entry = customtkinter.CTkEntry(master=booking_frame, placeholder_text="Enter number of seats")
    seats_entry.pack(pady=10)

    seat_numbers_label = customtkinter.CTkLabel(master=booking_frame, text="Seat Numbers:")
    seat_numbers_label.pack(pady=10)
    seat_numbers_entry = customtkinter.CTkEntry(master=booking_frame, placeholder_text="Enter seat numbers (e.g., '1 3, 1 4')")
    seat_numbers_entry.pack(pady=10)

    book_button = customtkinter.CTkButton(
        master=booking_frame,
        text="Confirm Booking",
        command=lambda: confirm_booking(customer_name_entry.get(), seats_entry.get(), seat_numbers_entry.get(), details_window, seat_matrix_label, flight_name)
    )
    book_button.pack(pady=20)

    # When the details window is closed, remove it from the dictionary
    details_window.protocol("WM_DELETE_WINDOW", lambda: close_details_window(flight_name, details_window))

def close_details_window(flight_name, details_window):
    details_window.destroy()  # Close the window
    del open_details_windows[flight_name]  # Remove the reference from the dictionary

def confirm_booking(customer_name, num_seats, seat_numbers, details_window, seat_matrix_label, flight_name):
    if not customer_name.strip():
        print("Customer name is required.")
        return
    
    seat_positions = []
    seat_numbers_list = seat_numbers.split(",")  # Split the input by commas to get multiple seat positions

    try:
        for sn in seat_numbers_list:
            row_col = sn.strip().split()  # Split each seat into row and column (e.g., "1 3")
            
            if len(row_col) != 2 or not row_col[0].isdigit() or not row_col[1].isdigit():
                raise ValueError("Invalid seat format. Expected row and column numbers.")

            row = int(row_col[0]) - 1  # Convert row to 0-based index
            col = int(row_col[1]) - 1  # Convert column to 0-based index
            seat_positions.append((row, col))

        # Call the C++ program to book the seats
        result = book_seats(customer_name, seat_positions, flight_name)

        if result:
            # Update the cached seat matrix for the flight
            seat_matrices_cache[flight_name] = result
            # Update the seat matrix label directly
            seat_matrix_label.configure(text="\n".join(seat_matrices_cache[flight_name]))
            # Update reservations
            update_reservations(customer_name, flight_name, seat_numbers)

        else:
            print("Booking failed.")

    except ValueError as e:
        print(f"Error in seat input: {e}")

def book_seats(customer_name, seat_positions, flight_name):
    seat_matrix = seat_matrices_cache[flight_name]  # Use the cached seat matrix
    
    # Mark the seats as booked in the seat matrix (1 for booked)
    updated_matrix = []
    for i, row in enumerate(seat_matrix):
        row_list = list(row)
        for pos in seat_positions:
            if pos[0] == i:
                row_list[pos[1]] = '1'  # Mark seat as booked
        updated_matrix.append("".join(row_list))
    
    # Update the cache
    return updated_matrix

def update_reservations(customer_name, flight_name, seat_numbers):
    if flight_name not in reservations:
        reservations[flight_name] = []
    reservations[flight_name].append((customer_name, seat_numbers))

# Global variable to keep track of the View Reservation window
view_reservation_window = None

# Function to open the View Reservations window
def open_view_reservations():
    global view_reservation_window

    if view_reservation_window is not None and view_reservation_window.winfo_exists():
        # If the window is already open, bring it to the front
        view_reservation_window.focus()
        return

    # Create a new window for viewing reservations
    view_reservation_window = customtkinter.CTkToplevel()
    view_reservation_window.geometry("800x600")
    view_reservation_window.title("View Reservations")

    # Loop through all flights and show their seat matrix and reservations
    for flight_name, flight_reservations in reservations.items():
        flight_label = customtkinter.CTkLabel(master=view_reservation_window, text=f"Flight: {flight_name}", font=("Roboto", 18))
        flight_label.pack(pady=10)

        # Get the final seat matrix for this flight
        seat_matrix = seat_matrices_cache.get(flight_name)
        if seat_matrix:
            seat_matrix_label = customtkinter.CTkLabel(master=view_reservation_window, text="\n".join(seat_matrix), font=("Roboto", 12))
            seat_matrix_label.pack(pady=5)

        # Display booked customers and their seat numbers
        if flight_reservations:
            reservations_label = customtkinter.CTkLabel(master=view_reservation_window, text="Reservations:", font=("Roboto", 16))
            reservations_label.pack(pady=5)
            for customer_name, seat_numbers in flight_reservations:
                customer_label = customtkinter.CTkLabel(master=view_reservation_window, text=f"{customer_name} booked seats: {seat_numbers}", font=("Roboto", 12))
                customer_label.pack(pady=2)

# Main interface for search
frame = customtkinter.CTkFrame(master=root)
frame.pack(pady=20, padx=60, fill="both", expand=True)

label = customtkinter.CTkLabel(master=frame, text="Flight Reservation System", font=("Roboto", 32))
label.pack(pady=12, padx=10)

# Create search label
search_label = customtkinter.CTkLabel(master=frame, text="Search Flights", font=("Roboto", 24))
search_label.pack(pady=12, padx=10)

# Create search entry (input field)
search_entry = customtkinter.CTkEntry(master=frame, placeholder_text="Enter flight or source city (e.g., NYC, LAX)")
search_entry.pack(pady=12, padx=10)

# Create search button
search_button = customtkinter.CTkButton(master=frame, text="Search", command=search_flights)
search_button.pack(pady=12, padx=10)

# Create a frame for search results (flight list)
results_frame = customtkinter.CTkFrame(master=frame)
results_frame.pack_forget()  # Initially hidden, will display after a search

# Create view reservations button
view_reservations_button = customtkinter.CTkButton(master=frame, text="View Reservations", command=open_view_reservations)
view_reservations_button.pack(pady=20)

# Start the main event loop
root.mainloop()
