import customtkinter
import tkinter as tk
import subprocess
import matplotlib.pyplot as plt
import networkx as nx

# Set appearance mode and color theme
customtkinter.set_appearance_mode("dark")
customtkinter.set_default_color_theme("dark-blue")

root = customtkinter.CTk()
root.geometry("850x650")

open_details_windows = {}
seat_matrices_cache = {}
reservations = {}
view_reservation_window = None

def draw_static_graph():
    # Create a sample graph for demonstration
    G = nx.Graph()
    G.add_edges_from([
    ("DEL", "BOM"),  # Delhi to Mumbai
    ("DEL", "BLR"),  # Delhi to Bangalore
    ("BOM", "HYD"),  # Mumbai to Hyderabad
    ("BLR", "HYD"),  # Bangalore to Hyderabad
    ("BOM", "CCU"),  # Mumbai to Kolkata
    ("CCU", "HYD"),  # Kolkata to Hyderabad
    ("HYD", "COK"),  # Hyderabad to Kochi
    ("COK", "TRV"),  # Kochi to Thiruvananthapuram
    ("BLR", "MAA"),  # Bangalore to Chennai
    ("DEL", "AMD"),  # Delhi to Ahmedabad
    ("DEL", "GOI"),  # Delhi to Goa
    ("JAI", "DEL")   # Jaipur to Delhi
])

    
    plt.figure(figsize=(8, 6))
    pos = nx.spring_layout(G)

    # Draw the graph
    nx.draw(G, pos, with_labels=True, node_size=2000, node_color='skyblue', font_size=16, font_color='black')
    plt.title("Flight Connections")
    plt.axis('off')
    plt.savefig("graph.png", format="png")  
    plt.close()

    graph_image = tk.PhotoImage(file="graph.png")

    
    graph_label = customtkinter.CTkLabel(master=frame, text="", image=graph_image)
    graph_label.image = graph_image
    graph_label.pack(pady=20)

def search_flights():
    query = search_entry.get().upper()
    print(f"Searching for flights from: {query}")
    results_frame.pack_forget()
    results_frame.pack(pady=12, padx=10, fill="both", expand=True)

    for widget in results_frame.winfo_children():
        widget.destroy()

    # Call the C++ program to get flight details
    result = subprocess.run(['./main_executable', 'flights', query], capture_output=True, text=True)

    if result.returncode != 0:
        print(f"Error fetching flights: {result.stderr}")
        return
    
    flight_details = result.stdout.strip().split('\n')
    for flight in flight_details:
        flight_button = customtkinter.CTkButton(master=results_frame, text=flight, command=lambda f=flight: open_flight_details(f))
        flight_button.pack(pady=5, padx=10)

def fetch_flights():
    try:
        
        result = subprocess.run(['./main_executable'], capture_output=True, text=True)
        
        
        if result.returncode != 0:
            print(f"Error fetching flights: {result.stderr}")
            return "Failed to fetch flight data."
        
        return result.stdout
    except Exception as e:
        print(f"Exception occurred while fetching flights: {e}")
        return "An error occurred."

def display_flights():
    flight_data = fetch_flights()
    results_label.configure(text=flight_data)

def open_flight_details(flight_name):
    if flight_name in open_details_windows:
        
        open_details_windows[flight_name].focus()
        return

    print(f"Opening details for flight: {flight_name}")
    details_window = customtkinter.CTkToplevel()
    details_window.geometry("800x600")
    details_window.title(f"Flight Details - {flight_name}")

    open_details_windows[flight_name] = details_window

    seat_matrix = seat_matrices_cache.get(flight_name)
    if seat_matrix is None:
        result = subprocess.run(['./main_executable', 'seats'], capture_output=True, text=True)

        if result.returncode != 0 or "Invalid" in result.stdout:
            print(f"Error fetching seat matrix: {result.stderr or result.stdout}")
            return

        seat_matrix = parse_seat_matrix(result.stdout.strip())
        seat_matrices_cache[flight_name] = seat_matrix
    
    
    seat_matrix_label = customtkinter.CTkLabel(master=details_window, text=format_seat_matrix(seat_matrix), font=("Roboto", 16))
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

    details_window.protocol("WM_DELETE_WINDOW", lambda: close_details_window(flight_name, details_window))

def close_details_window(flight_name, details_window):
    details_window.destroy()
    del open_details_windows[flight_name] 

def confirm_booking(customer_name, num_seats, seat_numbers, details_window, seat_matrix_label, flight_name):
    if not customer_name.strip():
        print("Customer name is required.")
        return
    
    seat_positions = []
    seat_numbers_list = seat_numbers.split(",")
    try:
        for sn in seat_numbers_list:
            row_col = sn.strip().split()  # Split each seat into row and column (e.g., "1 3")
            if len(row_col) != 2 or not row_col[0].isdigit() or not row_col[1].isdigit():
                raise ValueError("Invalid seat format. Expected row and column numbers.")
            row = int(row_col[0]) - 1  # Convert row to 0-based index
            col = int(row_col[1]) - 1  # Convert column to 0-based index
            seat_positions.append((row, col))
        
        result = book_seats(customer_name, seat_positions, flight_name)
        if result:
            seat_matrices_cache[flight_name] = result
            seat_matrix_label.configure(text=format_seat_matrix(result))
            update_reservations(customer_name, flight_name, seat_numbers)
        else:
            print("Booking failed.")
    except ValueError as e:
        print(f"Error in seat input: {e}")

def parse_seat_matrix(seat_matrix_str):
    return [[int(seat) for seat in row.split()] for row in seat_matrix_str.splitlines()]

def format_seat_matrix(seat_matrix):
    return "\n".join(" ".join(map(str, row)) for row in seat_matrix)

def book_seats(customer_name, seat_positions, flight_name):
    seat_matrix = seat_matrices_cache[flight_name]
    
    for pos in seat_positions:
        seat_matrix[pos[0]][pos[1]] = 1
    
    return seat_matrix

def update_reservations(customer_name, flight_name, seat_numbers):
    if flight_name not in reservations:
        reservations[flight_name] = []
    reservations[flight_name].append((customer_name, seat_numbers))

def open_view_reservations():
    global view_reservation_window
    if view_reservation_window is not None and view_reservation_window.winfo_exists():
        view_reservation_window.focus()
        return

    view_reservation_window = customtkinter.CTkToplevel()
    view_reservation_window.geometry("400x400")
    view_reservation_window.title("View Reservations")

    for flight, res in reservations.items():
        flight_label = customtkinter.CTkLabel(master=view_reservation_window, text=f"Flight: {flight}")
        flight_label.pack(pady=5)

        for name, seat in res:
            res_label = customtkinter.CTkLabel(master=view_reservation_window, text=f"Customer: {name}, Seats: {seat}")
            res_label.pack(pady=5)


results_frame = customtkinter.CTkFrame(master=root)
results_frame.pack(pady=12, padx=10, fill="both", expand=True)

# Create a frame for the main layout
frame = customtkinter.CTkFrame(master=root)
frame.pack(pady=20)

# Create search entry and buttons
search_entry = customtkinter.CTkEntry(master=frame, placeholder_text="Enter departure city")
search_entry.pack(pady=10, padx=20, fill=tk.X)

search_button = customtkinter.CTkButton(master=frame, text="Search Flights", command=search_flights)
search_button.pack(pady=10)

view_reservations_button = customtkinter.CTkButton(master=frame, text="View Reservations", command=open_view_reservations)
view_reservations_button.pack(pady=10)

# Draw the static graph
draw_static_graph()

# Start the application
root.mainloop()
