# Voting Management System
A system managing the voting process of an election using a **Self Adjusting Bloom Filter**, a **Red-Black Tree** and a **Linked List**.

Here's a visualization of the structures used in the voting management system:
![A visualization of the structures used in the voting management system](https://siatras.dev/img/voting-management-system.jpg)
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
- **Bloom Filter** is implemented using array of bits. It **resizes** when the filter’s **fill ratio reaches the maximum acceptable value**.

- **Postcode Struct** is implemented using a linked list of linked lists of pointer to records.

- **Red-Black Tree** is implemented using Binary Search Tree as basis.

- Each **Record** is represented by a simple struct of its data.

- **Election** is implemented using a struct. Having implemented struct Election offers the ability to extend the program easily to multiple elections at the same time by using, for example, an array of pointers to Elections.

## Implementation / Interface
- **Bloom Filter**: bf/bf.c, bf/bf.h

- **Linked list of pointers to records of the same postal code**: postcodelist/recordlist.c, postcodelist/recordlist.h
- **Linked list of recordlists to compose the Postcode Struct**: postcodelist/postcodelist.c, postcodelist/postcodelist.h

- **Red-Black Tree implemented using Binary Search Tree as basis**: rbt/rbt.c, rbt/rbt.h

- **Struct to define a record**: records/record.c, records/record.h

- **Utilities/functions used for arrays**: utils/array.c, utils/array.h
- **Utilities/functions used for arrays**: utils/file.c, utils/file.h
- **Utilities/functions used for prime numbers**: utils/primenumber.c, utils/primenumber.h

- **Struct to define an election**: election.c, election.h

- **Main function and some other helpful functions**: runelection.c
