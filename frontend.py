import customtkinter
import tkinter as tk
import subprocess

# Set appearance mode and color theme
customtkinter.set_appearance_mode("dark")
customtkinter.set_default_color_theme("dark-blue")

# Create main application window
root = customtkinter.CTk()
root.geometry("850x650")

# Function to handle search and display results
def search_flights():
    query = search_entry.get().upper()  # Get user input from search bar
    results_frame.pack_forget()  # Remove previous buttons before new search results
    results_frame.pack(pady=12, padx=10, fill="both", expand=True)
    
    # Clear previous search results
    for widget in results_frame.winfo_children():
        widget.destroy()

    # Call the C++ program to get flight details
    result = subprocess.run(['./main_executable', 'flights', query], capture_output=True, text=True)

    # Check for errors
    if result.returncode != 0:
        print("Error fetching flights:", result.stderr)
        return

    # Display the available flights returned by C++
    flight_details = result.stdout.strip().split('\n')
    for flight in flight_details:
        flight_button = customtkinter.CTkButton(master=results_frame, text=flight, command=lambda f=flight: open_flight_details(f))
        flight_button.pack(pady=5, padx=10)

# Function to open the flight details window
def open_flight_details(flight_name):
    details_window = customtkinter.CTkToplevel()
    details_window.geometry("800x600")
    details_window.title(f"Flight Details - {flight_name}")
    
    result = subprocess.run(['./main_executable', 'seats'], capture_output=True, text=True)

    if result.returncode != 0 or "Invalid" in result.stdout:
        print("Error fetching seat matrix:", result.stderr or result.stdout)
        return

    seat_matrix_output = result.stdout.strip().splitlines()
    seat_matrix_display = "\n".join(seat_matrix_output)

    seat_matrix_label = customtkinter.CTkLabel(master=details_window, text=seat_matrix_display, font=("Roboto", 16))
    seat_matrix_label.pack(pady=20)

    booking_frame = customtkinter.CTkFrame(master=details_window)
    booking_frame.pack(side=tk.RIGHT, padx=20, pady=20, fill=tk.BOTH, expand=True)

    seats_label = customtkinter.CTkLabel(master=booking_frame, text="Number of Seats:")
    seats_label.pack(pady=10)
    seats_entry = customtkinter.CTkEntry(master=booking_frame, placeholder_text="Enter number of seats")
    seats_entry.pack(pady=10)
    
    seat_numbers_label = customtkinter.CTkLabel(master=booking_frame, text="Seat Numbers:")
    seat_numbers_label.pack(pady=10)
    seat_numbers_entry = customtkinter.CTkEntry(master=booking_frame, placeholder_text="Enter seat numbers (e.g., 1A, 1B)")
    seat_numbers_entry.pack(pady=10)
    
    book_button = customtkinter.CTkButton(
        master=booking_frame,
        text="Confirm Booking",
        command=lambda: confirm_booking(seats_entry.get(), seat_numbers_entry.get(), details_window)
    )
    book_button.pack(pady=20)

def confirm_booking(num_seats, seat_numbers, details_window):
    seat_positions = []
    seat_numbers_list = seat_numbers.split(",")  # Get seat numbers from input

    try:
        for sn in seat_numbers_list:
            if not sn[:-1].isdigit() or len(sn) < 2 or not sn[-1].isalpha():
                raise ValueError("Invalid seat format")
            row = int(sn[:-1]) - 1  # Convert seat number to index (1-based to 0-based)
            col = ord(sn[-1].upper()) - 65  # Convert letter to column index (A=0, B=1, etc.)
            seat_positions.append((row, col))

        # Call the C++ program to book the seats
        book_seats(len(seat_positions), seat_positions, details_window)

    except ValueError as e:
        print(f"Error in seat input: {e}")

def book_seats(num_seats, seat_positions, details_window):
    # Build the command with seat booking data
    command = ['./main_executable', 'book', str(num_seats)]
    for seat in seat_positions:
        command.append(str(seat[0]))  # Row
        command.append(str(seat[1]))  # Column

    # Call the C++ executable with seat data
    result = subprocess.run(command, capture_output=True, text=True)

    # Check for errors
    if result.returncode != 0:
        print("Error booking seats:", result.stderr)
        return

    # Display the updated seat matrix returned by C++
    print("Booking result:", result.stdout)
    refresh_seat_matrix(details_window)

def refresh_seat_matrix(details_window):
    # Refresh the seat matrix in the flight details window
    details_window.destroy()
    open_flight_details("Your Flight Name Here")  # Pass the actual flight name if needed

# Main interface for search
frame = customtkinter.CTkFrame(master=root)
frame.pack(pady=20, padx=60, fill="both", expand=True)

label = customtkinter.CTkLabel(master=frame, text="Flight Reservation System", font=("Roboto", 32))
label.pack(pady=12, padx=10)

# Create search label
search_label = customtkinter.CTkLabel(master=frame, text="Search Flights", font=("Roboto", 24))
search_label.pack(pady=12, padx=10)

# Create search entry (input field)
search_entry = customtkinter.CTkEntry(master=frame, placeholder_text="Enter flight or destination")
search_entry.pack(pady=12, padx=10)

# Create search button
search_button = customtkinter.CTkButton(master=frame, text="Search", command=search_flights)
search_button.pack(pady=12, padx=10)

# Frame for displaying results
results_frame = customtkinter.CTkFrame(master=root)
results_frame.pack(pady=12, padx=10, fill="both", expand=True)

# Start the main loop
root.mainloop()
