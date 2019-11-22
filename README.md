# Voting Management System
A system managing the voting process of an election using a Bloom Filter, a Red-Black Tree and a Linked List.
![voting-management-system](https://siatras.dev/img/voting-management-system.jpg "voting-management-system")
## Compilation
`$ make clean`

`$ make`

## Usage
Command Line Parameters (and their flags) are optional and they can be used in any order.

`./runelection -i <Input File> -o <Output File> -n <Number of Updates Needed to Reconstruct Bloom Filter>`

## Commands
List of available commands:
- lbf [key] (lookup bloom-filter)
- lrb [key] (lookup red-black tree)
- ins [record]
- find [key]
- delete [key]
- vote [key]
- load [fileofkeys] (to vote)
- voted
- voted [postcode]
- votedperpc
- help
- exit

## Design Decisions
- **Bloom Filter** is implemented using array of bits.

- **Postcode Struct** is implemented using a linked list of linked lists of pointer to records.

- **Red-Black Tree** is implemented using Binary Search Tree as basis.

- Each **Record** is represented by a simple struct of its data.

- **Election** is implemented using a struct.

  Having implemented struct Election offers the ability to extend the program easily to multiple elections at the same time.
  
  eg. An array of pointers to Elections.

## Implementation / Interface
- bf/bf.c, bf/bf.h: Bloom Filter

- postcodelist/recordlist.c, postcodelist/recordlist.h: Linked list of pointers to records of the same postal code.
- postcodelist/postcodelist.c, postcodelist/postcodelist.h: Linked list of recordlists to compose the Postcode Struct.

- rbt/rbt.c, rbt/rbt.h: Red-Black Tree implemented using Binary Search Tree as basis.

- records/record.c, records/record.h: Struct to define a record.

- utils/array.c, utils/array.h: Utilities / functions used about arrays.
- utils/file.c, utils/file.h: Utilities / functions used about arrays.
- utils/primenumber.c, utils/primenumber.h: Utilities / functions used about prime numbers.

- election.c, election.h: Struct to define an election.

- runelection.c: Main function and some other helpful functions.