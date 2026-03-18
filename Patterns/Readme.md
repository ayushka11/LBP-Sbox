## Transformation for 6-bit
- Replace 2 by 1
- XOR every ith and (32+i)th row of the transpose of DDT 
- Replace 0 by 1

## Observations

### 1. Cyclic Shift
- Row 4 is a cyclic shift of **Row 2** by **3**
- Row 11 is a cyclic shift of **Row 8** by **8**
- Row 14 is a cyclic shift of **Row 9** by **12**

### 2. Column Blocks
(every row divided in 4 blocks of 8 each)
- Rows with all blocks identical - 2,3,4,5,13
- Other rows follow patterns like ABBA, ABAB or AABB

### 3. Row XOR Relations
- Row 1 = Row 5 XOR Row 7
- Row 1 = Row 6 XOR Row 10
- Row 1 = Row 9 XOR Row 15
- Row 2 = Row 4 XOR Row 13
- Row 2 = Row 7 XOR Row 11
- Row 3 = Row 4 XOR Row 5
- Row 3 = Row 9 XOR Row 14
- Row 4 = Row 2 XOR Row 13
- Row 4 = Row 3 XOR Row 5
- Row 5 = Row 1 XOR Row 7
- Row 5 = Row 3 XOR Row 4
- Row 5 = Row 6 XOR Row 8
- Row 5 = Row 12 XOR Row 15
- Row 6 = Row 1 XOR Row 10
- Row 6 = Row 5 XOR Row 8
- Row 6 = Row 11 XOR Row 14
- Row 6 = Row 13 XOR Row 15
- Row 7 = Row 1 XOR Row 5
- Row 7 = Row 2 XOR Row 11
- Row 7 = Row 8 XOR Row 10
- Row 7 = Row 9 XOR Row 12
- Row 8 = Row 5 XOR Row 6
- Row 8 = Row 7 XOR Row 10
- Row 8 = Row 12 XOR Row 13
- Row 9 = Row 1 XOR Row 15
- Row 9 = Row 3 XOR Row 14
- Row 9 = Row 7 XOR Row 12
- Row 9 = Row 10 XOR Row 13
- Row 10 = Row 1 XOR Row 6
- Row 10 = Row 7 XOR Row 8
- Row 10 = Row 9 XOR Row 13
- Row 11 = Row 2 XOR Row 7
- Row 11 = Row 6 XOR Row 14
- Row 12 = Row 5 XOR Row 15
- Row 12 = Row 7 XOR Row 9
- Row 12 = Row 8 XOR Row 13
- Row 13 = Row 2 XOR Row 4
- Row 13 = Row 6 XOR Row 15
- Row 13 = Row 8 XOR Row 12
- Row 13 = Row 9 XOR Row 10
- Row 14 = Row 3 XOR Row 9
- Row 14 = Row 6 XOR Row 11
- Row 15 = Row 1 XOR Row 9
- Row 15 = Row 5 XOR Row 12
- Row 15 = Row 6 XOR Row 13

