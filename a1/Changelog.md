
Re-Submission log note 
==========================================================================================================================================
NOTE:- All the Line number references are of old files. ( First submission)

In Python File - a1ece650.py

* Line 8 - Commented the Print Statement in the function "Output"
* After Line 10 - Added print statement for empty vertices and edges in the function "Output"
* After Line 14 - Added a variable E for storing egdes list, followed by multiple print statements to print the vertices and edges in the proper format abiding the rules.
* Line 19 - Removed the Print statement : print "Input:"
* After Line 27 - made the string variable 'street_name' lowercase. (That takes care of case insensitivity)
* After Line 33- Added a print statement : print "Error:Invalid Input Format"
* Line 41,44 and 47 - Commented out the Print Statements.


In Python File -custom.py 

* After Line 78 - Added an Else condition with a print statement : print "Error:The Street already exists."
* After Line 178 - Removed the function "output" as it was already present in the a1ece650.py file.
* Line 53 - Slight alteration of regular expression:

        Before - re_pat=r'([ac])\s("[a-zA-Z ]+")\s((\(-?\d+,-?\d+\))+)$' 
              * Did not include if space exist between the groupings.

        After  - re_pat=r'([ac])\s+?("[a-zA-Z ]+")\s+?((\(-?\d+,-?\d+\)\s*?)+)$'
              * Includes if zero or more spaces exist between the coordinates.

* Line 49 - Removed if condition in the function "Input_Parser". and kept that if condition after the Line 22 of a1ece650.py
* Line 32 - introduced round() to the distance method output.
* After Line 31 - added two methods called gradient and parallel.( To check if any two lines are parallel)
* Line 113 - index initialization variable removed and kept after the Line 110 ( intial point of the vertices_form method).
* Line 126 - removed the flag variable
* Line 127 to 132 - removed 'Try Except' structure and replaced with 'if not parallel' structure.
* Line 140 - added extra validation with 'and' operator.
* Line 141 to 145 - added '{0:.2f}' formatting to the values.
* Line 83 & 89 - changed the print statement.
* Line 165 - changed the variable name 'ii' to 'inter_ind'.
* After Line 171 - added varaible a1 and b1 that stores coordinates as list type parsing from Point type.
                 * Added two 'for loops' to find the index of the vertices to map edges.

* Line 174 & 175 - changed the variable names : a --> src_ind, b --> dst_ind, --> c --> inter_ind


