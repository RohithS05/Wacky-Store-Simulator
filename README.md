# Wacky Store Simulator 🛒

A C program that simulates a quirky store checkout system with customers, shopping carts, and checkout lanes.  
This project was part of coursework for **CSCB09: Software Tools and Systems Programming**.

## Features
- **Customer management**: create customers, add/remove items from carts, free memory.  
- **Shopping cart system**: supports adding duplicate items, edge cases like empty item names, and handling negative/invalid quantities.  
- **Checkout lanes**: enqueue/dequeue customers, process carts, handle multiple checkout lanes, and rebalance customers across lanes.  
- **Robust test suite**: includes 30+ regression tests (`R1–R30`) covering general, edge, and error cases.  

## File Structure
- `wackystore.c` → Core implementation (customer, cart, and checkout lane logic).  
- `main.c` → Test driver with unit tests and regression tests.  

## Example Run
```bash
gcc main.c wackystore.c -o wackystore
./wackystore
