                            �@ $      
DTRJA061.C                                                                                                                                                                                                   ��P             
  DTRJA061.C�  BACKUP/NOASSIST/COMMENT=VAX/VMS SPKITBLD Procedure/INTER/LOG/VERIFY DTR$KIT:[C]*.*; DTR$KIT:[000000]DTRJA061.C/LABEL=(DTRJA)/SAVE/BLOCK=9000/GROUP=25/NOINIT/NOREWI  VAX/VMS SPKITBLD Procedure  HAYASE        s P   `:-�*�      V5.5	 	 _JRDMIA:: 
   
  _$1$DUA31:  V5.5-2   $       �         �               * [C]CALCULATE.FOR;1 +  , �   .     / s P   4 M       �                    - 
    0  � 1    2   3      K � P   W   O     5   6  T,��"�  7  ��(�*�  8          9          G s P  H �� J �                            M C**************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C ( C  $ FORTRAN CORRELATE, PARSE, CALCULATE. C  $ LINK CORRELATE, PARSE, CALCULATE, DTR/OPT C  $ RUN CORRELATE C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.M C****************************************************************************  C C C****************************************************************** C C                    SUBROUTINE CALCULATE                         * C C Performs the calculations and prints out the linear regression  * C C equation and coefficient of correlation.                        * C C This program is a subroutine of the CORRELATE.FOR program       * C C******************************************************************    	SUBROUTINE CALCULATE()   A C****************************************************************  C 2 C       Formulas used to find the linear equation: C $ C	LINEAR EQUATION :  Y = bX + a					
 C									+ C	Equation to arrive at value for b:   				  C			(note:  E = summation				 ' C				n = number of data elements used)	 
 C									- C		b = E(X*Y) - n(average(X) * average(Y))			 - C		    -----------------------------------			 % C		    E(X**2) - n(average(x)**2)				 
 C									+ C	Equation to arrive at value for a:   				 
 C									' C		a = average(Y) - (b * average(X))			  C  C       Variables used:  C  C		E(X*Y) = SUMXY  C 		E(X**2) = SUMX2  C		N = COUNTER C A C****************************************************************  C = C       Formulas used to find the coefficient of correlation:  C + C			  a*E(Y) + b*E(X*Y) - n*(average(Y)**2) 9 C	CO_OF_CORR_SQRD = ------------------------------------- ' C			        E(Y**2) - n*(average(Y)**2)  C  C	COEFFICIENT OF CORRELATION:  C / C	CO_OF_CORR = square root of (CO_OF_CORR_SQRD)  C  C       Variables used:  C		E(Y) = SUMY C		E(X*Y) = SUMXY  C 		E(Y**2) = SUMY2  C		N = COUNTER C A C****************************************************************  C # C Include DAB and common variables.  C  	INCLUDE 'DTR$LIBRARY:DAB' 	INCLUDE 'DTR$LIBRARY:COMMON'  	CHARACTER*80 OUTPUT_LINE  	INTEGER*4 COUNTER/0/  	INTEGER*4 DTR_OPTS  	INTEGER*4 LEN1  	INTEGER*4 LEN2  	REAL*4 REC_BUFF(2)  	REAL*8 AVE(2) 	REAL*8 SUMY 	REAL*8 SUMXY  	REAL*8 SUMX2  	REAL*8 SUMY2  	REAL*8 TOP_LIN  	REAL*8 BOTTOM_LIN 	REAL*8 TOP_CORR 	REAL*8 BOTTOM_CORR  	REAL*8 CO_OF_CORR_SQRD  	REAL*8 CO_OF_CORR C A C Declare status SS$_NORMAL and the error message DTR$_UNWIND as   C external values. C  	EXTERNAL SS$_NORMAL 	EXTERNAL DTR$_UNWIND  C ( C Declare the default format for output. C  100	FORMAT (/1x,A) C  C Select DTR$DTR options.  C  	    DTR_OPTS =               5 	1         DTR$M_OPT_CMD   ! Return on DTR$K_STL_CMD	 / 	2	+ DTR$M_OPT_PGET  ! Return on DTR$K_STL_PGET  C = C Three STORE port-name statements were passed to DATA             TRIEVE  < C in the BEGIN-END block in the subroutine PARSE. The first ' C stored the count of records into PT1.  C 9 C If DATATRIEVE is at DTR$K_STL_PGET, retrieve the count   C from PT1.  C * 	IF (DAB$W_STATE .EQ. DTR$K_STL_PGET) then% 	    CALL DTR$GET_PORT (DAB, COUNTER)  	END IF  C	6 C While DATATRIEVE is at the stallpoint DTR$K_STL_MSG,7 C return to the main program if the user enters CTRL/C. 7 C Otherwise print out the message, write it to any open # C log file, then call DTR$CONTINUE.  C + 	DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG)  5 	    IF (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND)) THEN  		RETURN 	    END IF 6 	    IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN 		    WRITE (6, 100) MSG_BUFF * 	 	    CALL DTR$PUT_OUTPUT (DAB, MSG_BUFF) 	    END IF  	    CALL DTR$CONTINUE (DAB)   	END DO	     C	B C Check to see if there are no records that match the RSE entered.A C If there are no records, then there is no linear regression or   C coefficient of correlation.  C  	IF (COUNTER .EQ. 0) THEN  C @ C DATATRIEVE is at DTR$K_STL_PGET because of the two STORE PORT F C statements. Call DTR$GET_PORT until DATATRIEVE is at DTR$K_STL_MSG. @ C Then call DTR$CONTINUE until DATATRIEVE is at DTR$K_STL_CMD.   C G C Display error messages on the screen and to a file the user may have  	 C opened.  C 3 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_PGET) ' 		    CALL DTR$GET_PORT (DAB, REC_BUFF)  		END DO2 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG) 		    CALL DTR$CONTINUE (DAB)  		END DO 		WRITE (6, *)  . 	1	'0 Records Found, No valid regression line' 		CALL DTR$PUT_OUTPUT (DAB, / 	1	'0 Records Found, No valid regression line')  		RETURN 	END IF  C	= C Check to see if there was only 1 record found.  If there is 7 C only 1 record, then there is no linear regression or   C coefficient of correlation.  C  	IF (COUNTER .EQ. 1) THEN 3 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_PGET) ' 		    CALL DTR$GET_PORT (DAB, REC_BUFF)  		END DO2 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG) 		    CALL DTR$CONTINUE (DAB)  		END DO 		WRITE (6, *)  - 	1	'1 Record Found, No valid regression line'  		CALL DTR$PUT_OUTPUT (DAB, . 	1	'1 Record Found, No valid regression line') 		RETURN 	END IF  C % C Initialize the summation variables.  C  	SUMXY=0 	SUMX2=0 	SUMY2=0 C F C If DATATRIEVE is at DTR$K_STL_PGET, it has stored the totals of the A C fields into PT2. Get the totals in the port and pass them to a   C variable in the program. C * 	IF (DAB$W_STATE .EQ. DTR$K_STL_PGET) then& 	    CALL DTR$GET_PORT (DAB, REC_BUFF) 	END IF  C	6 C While DATATRIEVE is at the stallpoint DTR$K_STL_MSG,7 C return to the main program if the user enters CTRL/C. 7 C Otherwise print out the message, write it to any open # C log file, then call DTR$CONTINUE.  C + 	DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG)  5 	    IF (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND)) THEN  		RETURN 	    END IF 6 	    IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN 		    WRITE (6, 100) MSG_BUFF * 	 	    CALL DTR$PUT_OUTPUT (DAB, MSG_BUFF) 	    END IF  	    CALL DTR$CONTINUE (DAB)   	END DO	     C D C Move the total of FIELD2 into the variable for the summation of y. C  	SUMY = REC_BUFF(2)  C , C Find the average values of the two fields. C " 136	AVE(1) = REC_BUFF(1) / COUNTER 	AVE(2) = REC_BUFF(2) / COUNTER  C B C If DATATRIEVE is at DTR$K_STL_PGET, it has stored the values for) C individual fields in the record stream.  C . 140	DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_PGET) C ? C Get the field value and pass it to a variable in the program.  C & 	    CALL DTR$GET_PORT (DAB, REC_BUFF) C 9 C Compute the sum of the two variables after multiplying  5 C them. Compute the sum of the squares of FIELD1 and  	 C FIELD2.  C . 	    SUMXY = SUMXY + (REC_BUFF(1)*REC_BUFF(2))& 	    SUMX2 = SUMX2 + (REC_BUFF(1)**2) % 	    SUMY2 = SUMY2 + (REC_BUFF(2)**2)  	END DO  C	9 C If the user entered CTRL/C, return to the main program.  C + 	IF ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .AND. 3 	1   (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND))) THEN  		RETURN 	END IF  C 3 C Finish computation of the linear regression line.  C - 	TOP_LIN = (SUMXY - (COUNTER*AVE(1)*AVE(2)))	 - 	BOTTOM_LIN = (SUMX2 - (COUNTER*(AVE(1)**2)))  C D C If all values of FIELD1 are the same, then there is no regression & C line. Display a message to the user. C  	IF (BOTTOM_LIN .EQ. 0) THEN8 		WRITE (6, *)'There is no valid linear regression line' 		CALL DTR$PUT_OUTPUT (DAB, . 	1	'There is no valid linear regression line')2 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG) 		    CALL DTR$CONTINUE (DAB)  		END DO 		RETURN 	END IF  	B = TOP_LIN / BOTTOM_LIN  	A = AVE(2) - (B * AVE(1) )  C 3 C Finish computation of coefficient of correlation.  C : 	TOP_CORR = ((A*SUMY) + (B*SUMXY) - (COUNTER*(AVE(2)**2))). 	BOTTOM_CORR = (SUMY2 - (COUNTER*(AVE(2)**2))) C G C If all values of FIELD2 are the same, then there is no coefficient of - C correlation. Display a message to the user.  C  	IF (BOTTOM_CORR .EQ. 0) THEN - 		WRITE (6, *)'There is no valid coefficient   	1of correlation'  		p                                                                                                                                                           ��q $      
DTRJA061.C                     �  
  [C]CALCULATE.FOR;1                                                                                                             M                              ��             CALL DTR$PUT_OUTPUT (DAB, 3 	1	'There is no valid coefficient of correlation')  2 	        DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_MSG) 		    CALL DTR$CONTINUE (DAB)  		END DO 		RETURN 	END IF ) 	CO_OF_CORR_SQRD = TOP_CORR / BOTTOM_CORR % 	CO_OF_CORR = DSQRT (CO_OF_CORR_SQRD)  C	9 C If the user entered CTRL/C, return to the main program.  C + 	IF ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .AND. 3 	1   (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND))) THEN  		RETURN 	END IF  C A C Retrieve the name of the first field from the auxiliary buffer. 8 C Save the length of the field name in another variable. C  	FIELD1 = AUX_BUFF 	LEN1 = DAB$W_AUX_LEN  	CALL DTR$CONTINUE (DAB) C	9 C If the user entered CTRL/C, return to the main program.  C + 	IF ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .AND. 3 	1   (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND))) THEN  		RETURN 	END IF  C B C Retrieve the name of the second field from the auxiliary buffer.8 C Save the length of the field name in another variable. C  	FIELD2 = AUX_BUFF 	LEN2 = DAB$W_AUX_LEN  	CALL DTR$CONTINUE (DAB) C	9 C If the user entered CTRL/C, return to the main program.  C + 	IF ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .AND. 3 	1   (DAB$L_CONDITION .EQ. %LOC(DTR$_UNWIND))) THEN  		RETURN 	END IF  C / C Print headers for the linear regression line.  C  	WRITE (6, 220) 8 220	FORMAT (//7X, 'LINEAR'/5X, 'REGRESSION'/8X, 'LINE'/) C @ C Print headers for the linear regression line to a file if one  C was opened.  C   	CALL DTR$PUT_OUTPUT (DAB, '  ')  	CALL DTR$PUT_OUTPUT (DAB, '  '), 	CALL DTR$PUT_OUTPUT (DAB, '        LINEAR'). 	CALL DTR$PUT_OUTPUT (DAB, '      REGRESSION')+ 	CALL DTR$PUT_OUTPUT (DAB, '         LINE')   	CALL DTR$PUT_OUTPUT (DAB, '  ') C # C Print the linear regression line.  C 6 	WRITE (6, 225) FIELD2 (1:LEN2), A, B, FIELD1 (1:LEN1)7 225	FORMAT (1X, A, '= ', F20.4, ' + ', F20.4, ' * ', A) @ 	WRITE (OUTPUT_LINE, 225) FIELD2 (1:LEN2), A, B, FIELD1 (1:LEN1)' 	CALL DTR$PUT_OUTPUT (DAB, OUTPUT_LINE)  C 3 C Print headers for the coefficient of correlation.  C  	WRITE (6, 230)*< 230	FORMAT (//5X, 'COEFFICIENT'/9X, 'OF'/5X, 'CORRELATION'/)  	CALL DTR$PUT_OUTPUT (DAB, '  ')  	CALL DTR$PUT_OUTPUT (DAB, '  ')/ 	CALL DTR$PUT_OUTPUT (DAB, '      COEFFICIENT')C* 	CALL DTR$PUT_OUTPUT (DAB, '          OF')/ 	CALL DTR$PUT_OUTPUT (DAB, '      CORRELATION')E  	CALL DTR$PUT_OUTPUT (DAB, '  ') Ct' C Print the coefficient of correlation.  Co 	WRITE (6, 235) CO_OF_CORR 235	FORMAT (7X, F7.5)a$ 	WRITE (OUTPUT_LINE, 235) CO_OF_CORR' 	CALL DTR$PUT_OUTPUT (DAB, OUTPUT_LINE)* C*: C Leave a blank line after the coefficient of correlation. C* 	WRITE (6, *)' '  	CALL DTR$PUT_OUTPUT (DAB, '  ') CN C Return to the main program.  C 
 300	RETURN 	END��alculations and prints out the linear regression  * C C equation and coefficient of correlation.                        * C C This program is a subroutine of the CORRELATE.FOR program       * C C******************************************************************    	SUBROUTINE CALCULATE()   A C*******************************************�               * [C]CHOOSE.FOR;1 +  , �   .     / s P   4 O       �                    - 
    0  � 1    2   3      K � P   W   O     5   6  ���"�  7 `�)�*�  8          9          G s P  H �� J �   
            K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C G C********************************************************************** 2 C                   SUBROUTINE CHOOSE  				      *G C The program shows the domains available in the current dictionary   * G C and prompts the user to ready a domain.                             * G C If the domain name is invalid or the domain cannot be readied, the  * G C program reprompts for a domain name.                                * G C This program is a subroutine of the MENU.FOR program                * G C**********************************************************************    	SUBROUTINE CHOOSE (DOMAIN)  	INCLUDE 'DTR$LIBRARY:DAB' 	CHARACTER*31 DOMAIN 	LOGICAL      NO_DOMAIN/.TRUE./    	DO WHILE (NO_DOMAIN) , 	    CALL DTR$COMMAND (DAB, 'SHOW DOMAINS;')  4 C Use DTR$DTR to display the result of SHOW DOMAINS.  & 	    CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  + C Ask the user for the domain and ready it.    	    WRITE (6, 20)C 20	    FORMAT (' Enter the name of the domain you want to use: ',$)  	    READ (5, 1) DOMAIN  1	    FORMAT (A)5 		CALL DTR$COMMAND (DAB, 'READY !CMD WRITE;', DOMAIN)   C C Check for an error in readying the domain; reprompt if there was. F C Form a collection of all records in the domain and check for errors.  # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)   3 		IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN  		    WRITE (6, 40) ! 40		    FORMAT (' Try again....')  		ELSE 		    NO_DOMAIN = .FALSE. 2 		    CALL DTR$COMMAND (DAB, 'FIND !CMD;', DOMAIN) 		    CALL MESSAGE  7 		    IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN  			NO_DOMAIN = .TRUE.  		    END IF 		END IF 	END DO          NO_DOMAIN = .TRUE.   	RETURN  	END��                                                                                                                                                                                                                                                                                                                  �               * [C]COLUMNS.BAS;1 +  , �   .     / s P   4 P       h   �                - 
    0  � 1    2   3      K � P   W   O     5   6 ���{ )�  7 `	)�*�  8          9          G s P  H �� J �  
             050	%TITLE "COLUMNS"O 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! K 	! To run this program, you must compile it and link it with the DATATRIEVE  	! shareable image:  	!   	!  $ BASIC COLUMNS  	!  $ LINK COLUMNS, DTR/OPT  	!  $ RUN COLUMNS  	!H 	! See the VAX DATATRIEVE Guide to Programming and Customizing for more M 	! information on using an option file to link programs that call DATATRIEVE. O 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  	!J 	!The program COLUMNS creates a  2-column  report  of data in a DATATRIEVEL 	!domain. The program prompts for the name of the domain, a record selectionJ 	!expression, and the names of the fields that you want in the report. TheJ 	!program then displays the report on the screen and writes it to the file 	!REPORT.LIS.  	!M 	!You  may want to edit the program and change parameters, such as the number : 	!and width of columns and the number of lines  per  page.P 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" 100	%INCLUDE "DTR$LIBRARY:DAB.BAS"+ 	! Declare the buffers for the report file.   5 	COMMON (RPT_BUFFERS) STRING BIG_BUFF(2%,35%) = 35, &  				    HEADERS (5%) = 35   1 	! Declare the initialization call as a function.  	! Declare the success status.  # 	EXTERNAL INTEGER FUNCTION DTR$INIT # 	EXTERNAL LONG CONSTANT SS$_NORMAL	   , 	! Declare the options for the DTR$DTR call.   	DECLARE INTEGER DTR_OPTIONS. 	DTR_OPTIONS = DTR$M_OPT_CMD + DTR$M_OPT_LINE    	DECLARE INTEGER RET_STATUS    	! Open the report file.  , 	OPEN "REPORT.LIS" FOR OUTPUT AS FILE #1%, &+         SEQUENTIAL VARIABLE, RECORDSIZE 80%    	! Initialize the interface   ? 500	RET_STATUS = DTR$INIT (DAB BY REF, 100% BY REF, MSG_BUFF, & ( 		AUX_BUFF, DTR$K_SEMI_COLON_OPT BY REF)  7 	! Verify that DATATRIEVE was initialized successfully.   " 	IF RET_STATUS <> SS$_NORMAL THEN 0 	     PRINT "DATATRIEVE initialization failed."  	     GOTO 8100    	! Clear the screen.' 	! Show the user the domains available.   ' 525	CALL LIB$ERASE_PAGE BY REF (1%, 1%) 1 550	CALL DTR$COMMAND (DAB BY REF, "SHOW DOMAINS")   6 	! Use DTR$DTR to display the results of SHOW DOMAINS.  ) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   8 	! Prompt the user for domain name and ready the domain.  $ 600	INPUT "What is the domain"; DOM$2 	CALL DTR$COMMAND (DAB BY REF, "READY !CMD", DOM$)  = 	! Use DTR$DTR to handle messages and return at DTR$K_STL_CMD   ) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)    	! Check for errors.  ) 	IF DA�                                                                                                                                                                                                                                                                           N�� $      
DTRJA061.C                     �  
  [C]COLUMNS.BAS;1                                                                                                               P   �                          1�             B$L_CONDITION <> DTR$_SUCCESS THEN  % 		CALL LIB$ERASE_PAGE BY REF (1%, 1%)  		PRINT "Try again....."
 		GOTO 550 	 3 	! Show the available fields for the domain chosen.   < 700	CALL DTR$COMMAND (DAB BY REF, "SHOW FIELDS !CMD", DOM$) ) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   % 	! Prompt for an RSE and field names.   0 	PRINT "Finish the record selection expression." 	LINPUT "FIND "; A$ ) 	LINPUT "Enter a list of field names"; B$   2 	! Instruct DATATRIEVE to print the chosen fields.. 	! Use DTR$DTR to print out the record stream.  6 	CALL DTR$COMMAND (DAB BY REF, "FOR !CMD PRINT !CMD",& 	    A$, B$)' 	CALL DTR$DTR BY REF (DAB, DTR_OPTIONS)    	! Check for errors.  $ 	IF DAB$W_STATE = DTR$K_STL_CMD THEN 		PRINT " TRY AGAIN " 
 		GOTO 700    @ 800	CALL DTR$CONTINUE (DAB BY REF)	 ! Skip the first blank line.  ( 	! Set the counter of header lines to 0.   	INC% = 0%		  0 	! Move the header lines into the header buffer.   	WHILE DAB$W_MSG_LEN <> 0% 		INC% = INC% + 1% 		HEADERS(INC%) = MSG_BUFF    		CALL DTR$CONTINUE (DAB BY REF) 	NEXT < 	CALL DTR$CONTINUE (DAB BY REF)  ! Skip the last blank line.  - 	! Fill the report buffer one side at a time.    2000	FOR I% = 1% TO 2%		 2100	    FOR J% = 1% TO 35%		   4 	! Move the contents of the message buffer into the : 	! report buffer. If there are no more records, clear out  	! the rest of the buffer.  ' 		IF DAB$W_STATE = DTR$K_STL_LINE THEN   			BIG_BUFF(I%,J%) = MSG_BUFF " 			CALL DTR$CONTINUE (DAB BY REF)  		ELSE BIG_BUFF(I%,J%) = "   " 2200	    NEXT J% 2300	NEXT I%   	! Display the report file. 8 	! Print one set of headers if there is only one column.  ( 2600	CALL LIB$ERASE_PAGE BY REF (1%, 1%) 	FOR K% = 1% TO INC%) 	    IF BIG_BUFF(2%,1%) = "     "  THEN &  		 PRINT #1%, HEADERS(K%)  	 	 PRINT HEADERS(K%) 	 	    ELSE 2 		 PRINT #1%, HEADERS(K%) + "     " + HEADERS(K%) 4 	         PRINT HEADERS(K%) + "     " + HEADERS(K%)  2800	NEXT K%  % 	! Insert 1 blank line after headers.    2850	PRINT #1%, "   "  2875	PRINT "   "  ( 	! Write out the report file REPORT.LIS.9 	! Stop when there are no more records for the left side.    2900	FOR J% = 1% TO 35% * 	    IF BIG_BUFF(1%, J%) = "     "  THEN & 		GO TO 2950	 	    ELSE 8 		PRINT #1%, BIG_BUFF(1%,J%) + "     " + BIG_BUFF(2%,J%)3 		PRINT BIG_BUFF(1%,J%) + "     " + BIG_BUFF(2%,J%)  2950	NEXT J%  - 	! At the end of the page, print a form feed.     + 3200	IF DAB$W_STATE = DTR$K_STL_MSG  THEN & ( 		CALL DTR$DTR BY REF (DAB, DTR_OPTIONS)+ 3300	IF DAB$W_STATE = DTR$K_STL_LINE THEN &  		PRINT #1%, "<FF>"  		GOTO 2000   & 	! Finish the session with DATATRIEVE.  ! 8000	CALL DTR$FINISH (DAB BY REF)  8100	END��                                                                                                                                                                                                                                                                                                                                                                                                                      �               * [C]COMMON.FOR;1 +  , ] !   .     / s P   4 -       �   �               - 
    0  � 1    2   3      K � P   W   O     5   6  ��N�b�  7  f))�*�  8          9          G s P  H �� J �                	CHARACTER*31 FIELD1 	CHARACTER*1  COMMA  	CHARACTER*31 FIELD2 	CHARACTER*2  WORD_OF  	CHARACTER*80 EXPRESSION 	INTEGER*4    INIT_OPTS - 	COMMON FIELD1, FIELD2, EXPRESSION, INIT_OPTS   ��                                                                                                                                                                                                                                                                                                                              �               * [C]CORRELATE.FOR;1 +  , {    .     / s P   4 N       j                   - 
    0  � 1    2   3      K � P   W   O     5   6 `{���"�  7 `�8)�*�  8          9          G s P  H �� J �                            M C**************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C ( C  $ FORTRAN CORRELATE, PARSE, CALCULATE. C  $ LINK CORRELATE, PARSE, CALCULATE, DTR/OPT C  $ RUN CORRELATE C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.M C****************************************************************************  C F C The program CORRELATE.FOR shows you how you can add the user-defined. C keyword CORRELATE to interactive DATATRIEVE.C C Users who run the program can use DATATRIEVE in the usual manner. 4 C In addition, they can use the statement CORRELATE." C The format of this statement is: C : C CORRELATE independent-field [,] dependent-field [OF rse] C 8 C The following examples show how you can use CORRELATE: C  C DTR> READY ANNUAL_REPORT6 C DTR> CORRELATE RESEARCH, NET_INCOME OF ANNUAL_REPORT C  C          LINEAR  C        REGRESSION  C           LINE C G C   NET_INCOME =             -11.5668 +               1.3894 * RESEARCH  C  C        COEFFICIENT C            OF  C        CORRELATION C  C         0.99749  C 8 C DTR> FIND ANNUAL_REPORT WITH DATE BEFORE "01-JAN-1978" C [7 records found]  C DTR> BEGIN' C CON>    CORRELATE EMPLOYEES, SERVICES 3 C CON>    CORRELATE EMPLOYEES, NET_INCOME_PER_SHARE 
 C CON> END C          LINEAR  C        REGRESSION  C           LINE C F C   SERVICES =            -112.9101 +               0.0132 * EMPLOYEES C  C        COEFFICIENT C            OF  C        CORRELATION C  C         0.80055  C          LINEAR  C        REGRESSION  C           LINE C G C   NET_INCOME_PER_SHARE =       -1.1342 +           0.0002 * EMPLOYEES  C  C        COEFFICIENT C            OF  C        CORRELATION C  C         0.82269  C  C DTR> C ; C One program and two subroutines create the UDK CORRELATE:  C G C o The main program, CORRELATE.FOR, initializes DATATRIEVE and invokes K C   the terminal server. The program simulates interactive DATATRIEVE until < C   the user enters CORRELATE. The program then calls PARSE. C    C o The subroutine PARSE:  C % C   - Parses the CORRELATE statement.  C A C   - Converts each CORRELATE statement the user entered into one % C     DATATRIEVE BEGIN-END statement. 4 C     For example, if the user enters the statement: C 5 C     CORRELATE RESEARCH, NET_INCOME OF ANNUAL_REPORT  C N C     The subroutine PARSE converts it into the following BEGIN-END statement: C  C     BEGIN  C       DECLARE PORT PT1 USING C         01 NUM PIC 9(4) COMP.  C       DECLARE PORT PT2 USING C         01 WHOLE.  C            02 PART_A REAL. C            02 PART_B REAL.4 C       STORE PT1 USING NUM = COUNT OF ANNUAL_REPORT C       STORE PT2 USING  C         BEGIN 4 C           PART_A = TOTAL RESEARCH OF ANNUAL_REPORT6 C           PART_B = TOTAL NET_INCOME OF ANNUAL_REPORT C         END  C       FOR ANNUAL_REPORT  C         BEGIN  C           STORE PT2 USING  C             BEGIN ! C               PART_A = RESEARCH # C               PART_B = NET_INCOME  C             END  C         END  C       DISPLAY RESEARCH C       DISPLAY NET_INCOME	 C     END  C 3 C   - Passes the BEGIN-END statement to DATATRIEVE.  C * C   - Returns control to the main program. C C C o The subroutine CALCULATE uses the values passed in the ports to K C   calculate the linear regression and coefficient of correlation. It then  C   displays the results.  C H C The main program and the subroutines include a file called COMMON.FOR.? C This file contains variable declarations common to all three. 1 C Here is the file COMMON.FOR:                     C  C       CHARACTER*31 FIELD1  C       CHARACTER*1  COMMA C       CHARACTER*31 FIELD2  C       CHARACTER*2  WORD_OF C       CHARACTER*80 EXPRESSION  C       INTEGER*4    INIT_OPTS4 C       COMMON FIELD1, FIELD2, EXPRESSION, INIT_OPTS C M C**************************************************************************** - C                   C O R R E L A T E . F O R  C + C Include the DAB and the common variables.  C  	INCLUDE  'DTR$LIBRARY:DAB'  	INCLUDE  'DTR$LIBRARY:COMMON' 	INTEGER*4 DTR$DTR 	INTEGER*4 DTR$GET_STRING  	INTEGER*2 DTR_OPTS  	INTEGER   RET_STATUS  C % C Declare the normal and exit status.  C  	EXTERNAL SS$_NORMAL 	EXTERNAL DTR$_EXIT  C < C Select options and initialize the session with DATATRIEVE. C ! 	    INIT_OPTS =                   	1	+ DTR$K_SEMI_COLON_OPT    	2	+ DTR$K_UNQUOTE�                                                                                                                                                                                                                                                           R��� $      
DTRJA061.C                     {   
  [C]CORRELATE.FOR;1                                                                                                             N                              �^ 
     
       D_LIT  	3	+ DTR$K_FORMS_ENABLE  	4	+ DTR$K_SYNTAX_PROMPT  8 	CALL DTR$INIT (DAB, 100, MSG_BUFF, AUX_BUFF, INIT_OPTS) C E C Use the DTR$CREATE_UDK call to create the UDKs CORRELATE and SPAWN.  C > 	CALL DTR$CREATE_UDK (DAB, 'CORRELATE', 1,DTR$K_UDK_STATEMENT)9 	CALL DTR$CREATE_UDK (DAB, 'SPAWN', 2, DTR$K_UDK_COMMAND)  C # C Declare the DTR$DTR call options.  C  	DTR_OPTS = 3 	1	+ DTR$M_OPT_PGET      ! Return on DTR$K_STL_PGET 5 	2 	+ DTR$M_OPT_CONTROL_C ! Enable Control C handling 8 	3 	+ DTR$M_OPT_STARTUP   ! Execute startup command file< 	4 	+ DTR$M_OPT_FOREIGN   ! Execute invocation command lines- 	5	+ DTR$M_OPT_BANNER    ! Display DTR banner 3 	6 	+ DTR$M_OPT_UDK       ! Return on DTR$K_STL_UDK 6 	7	+ DTR$M_OPT_END_UDK   ! Return on DTR$K_STL_END_UDK C ; C Continue until the user has entered EXIT or CTRL/Z at the  C DTR> prompt. C , 	DO WHILE (RET_STATUS .NE. %LOC(DTR$_EXIT))  C & C Call the DATATRIEVE terminal server. C ' 	  RET_STATUS = DTR$DTR (DAB, DTR_OPTS)  C D C If the terminal server call returns a status other than success or C exit, then stop the program. C 0 	  IF ((RET_STATUS .NE. %LOC(SS$_NORMAL)) .AND. . 	1     (RET_STATUS .NE. %LOC(DTR$_EXIT))) THEN2 		WRITE (6, *) 'Error in calling terminal server.' 		STOP	 	  END IF  C C C If the user enters a UDK, the program gets control at this point. 3 C If the UDK entered is CORRELATE, then call PARSE.  C E C The PARSE subroutine retrieves the rse the user entered and parses  E C it. The subroutine then passes a BEGIN-END statement to DATATRIEVE  - C in place of the arguments the user entered.  C = C DATATRIEVE checks the statement for errors and executes it.  C + 	  IF (DAB$W_STATE .EQ. DTR$K_STL_UDK) THEN & 	     IF (DAB$W_UDK_INDEX .EQ. 1) THEN 		CALL PARSE() 	     END IF C 7 C Call a Run-Time Library routine to spawn a subprocess  C if the user entered SPAWN. C & 	     IF (DAB$W_UDK_INDEX .EQ. 2) THEN 		CALL LIB$SPAWN() 	     END IF C ! C Terminate the UDK and continue.  C  	     CALL DTR$END_UDK (DAB)	 	  END IF  C < C DATATRIEVE returns to the program with the DTR$K_STL_PGET  C stallpoint.  C A C Call the subroutine CALCULATE to handle the DATATRIEVE commands  C issued by PARSE.FOR. C , 	  IF (DAB$W_STATE .EQ. DTR$K_STL_PGET) THEN 	    CALL CALCULATE() 	 	  END IF    	END DO  C 0 C If EXIT or CTRL/Z is entered, end the session. C  999	CALL DTR$FINISH (DAB)    	END��                                                                                                                                                    �               * [C]DAB.BAS;1 +  , � /   .     / s P   4 8       t   �                - 
    0  � 1    2   3      K � P   W   O     5   6 �V-IhA�  7  N)�*�  8          9 @����  G s P  H �� J �                   	MAP (ACCESSBLOCK) & 	BYTE DAB$B_BID, & 	BYTE DAB$B_BLN, & 	LONG DAB$L_CONDITION, & 	LONG DAB$A_MSG_BUF, & 	WORD DAB$W_MSG_BUF_LEN, & 	WORD DAB$W_MSG_LEN, & 	LONG DAB$A_AUX_BUF, & 	WORD DAB$W_AUX_BUF_LEN, & 	WORD DAB$W_AUX_LEN, & 	WORD DAB$W_IDI, & 	WORD DAB$W_STATE, & 	LONG DAB$L_FLAGS, & 	LONG DAB$L_OPTIONS, & 	WORD DAB$W_REC_LENGTH, &  	WORD DAB$W_VERSION, & 	WORD DAB$W_LEVEL, & 	BYTE DAB$B_VER_LETTER, &  	WORD DAB$W_BASE_LEVEL, &  	WORD DAB$W_UDK_INDEX, & 	WORD DAB$W_COLUMNS_PAGE, &  	WORD DAB$W_TT_CHANNEL, &  	WORD DAB$W_CTLC_CHANNEL, &  	LONG DAB$L_KEYTABLE_ID, & 	LONG DAB$L_COMMAND_KEYBOARD, &  	LONG DAB$L_PROMPT_KEYBOARD, & 	LONG DAB$A_LOG_RAB, & 	LONG DAB$L_LMF_CONTEXT, & 	LONG DAB$L_RETURN_VALUE, &  	WORD DAB$W_MAIN_WINDOW_X, & 	WORD DAB$W_MAIN_WINDOW_Y   # 	MAP (ACCESSBLOCK) STRING DAB = 100   / 	DECLARE INTEGER CONSTANT DTR$K_STL_CMD = 1% ,&  				 DTR$K_STL_PRMPT = 2%,&  				 DTR$K_STL_LINE = 3%,& 				 DTR$K_STL_MSG = 4%,&  				 DTR$K_STL_PGET = 5%,& 				 DTR$K_STL_PPUT = 6%,& 				 DTR$K_STL_CONT = 7%,& 				 DTR$K_STL_UDK = 8%,&  				 DTR$K_STL_END_UDK = 9%   6 	DECLARE INTEGER CONSTANT DTR$K_SEMI_COLON_OPT = 1%, &  				 DTR$K_UNQUOTED_LIT = 16%, &! 				 DTR$K_SYNTAX_PROMPT = 32%, &   				 DTR$K_IMMED_RETURN = 64%, &! 				 DTR$K_FORMS_ENABLE = 128%, &  				 DTR$K_VERIFY = 256%, & $ 				 DTR$K_CONTEXT_SEARCH = 2048%, &( 		   		 DTR$K_HYPHEN_DISABLED = 4096%, && 		   		 DTR$K_MORE_COMMANDS = 8192%, & 		   		 DTR$K_ABORT = 16384%, &  				 DTR$K_LOCK_WAIT = 32768%   / 	DECLARE INTEGER CONSTANT DTR$M_OPT_CMD = 1%, &  				 DTR$M_OPT_PRMPT = 2%, & 				 DTR$M_OPT_LINE = 4%, &  				 DTR$M_OPT_MSG = 8%, & 				 DTR$M_OPT_PGET = 16%, & 				 DTR$M_OPT_PPUT = 32%, & 				 DTR$M_OPT_CONT = 64%, & 				 DTR$M_OPT_UDK = 128%, &  				 DTR$M_OPT_DTR_UDK = 256%, &  				 DTR$M_OPT_END_UDK = 512%, &  				 DTR$M_OPT_UNWIND = 1024%, &# 				 DTR$M_OPT_CONTROL_C = 2048%, & ! 				 DTR$M_OPT_STARTUP = 4096%, & ! 				 DTR$M_OPT_FOREIGN = 8192%, & ! 				 DTR$M_OPT_BANNER = 16384%, & & 				 DTR$M_OPT_REMOVE_CTLC = 32768%, & 				 DTR$M_OPT_KEYDEFS = 65536%   / 	DECLARE INTEGER CONSTANT DTR$K_UDK_SET = 1%, &  				 DTR$K_UDK_SET_NO = 2%, &  				 DTR$K_UDK_SHOW = 3%, &   				 DTR$K_UDK_STATEMENT = 4%, & 				 DTR$K_UDK_COMMAND = 5%  				  1 	DECLARE INTEGER CONSTANT DTR$K_TOK_TOKEN = 1%, &  				 DTR$K_TOK_PICTURE = 2%, & 				 DTR$K_TOK_FILENAME = 3%, &  				 DTR$K_TOK_COMMAND = 4%, &! 				 DTR$K_TOK_TEST_TOKEN = 5%, & # 				 DTR$K_TOK_LIST_ELEMENT = 6%, &  				 DTR$K_TOK_TEST_EOL = 7% 				  8 	COMMON(DTR$BUFFERS) STRING MSG_BUFF = 80, AUX_BUFF = 20  ( 	EXTERNAL INTEGER CONSTANT DTR$_SUCCESS     ��                                                                                                                                                                                                                                                                                                                                                                                                          �               * [C]DAB.FOR;1 +  , � -   . 	    / s P   4 A   	    �  �                - 
    0  � 1    2   3      K � P   W   O 	    5   6 ��*���  7 �T`)�*�  8          9 @����  G s P  H �� J �      
             c ! c FORTRAN DATATRIEVE Access Block  c   ; 	LOGICAL*1 DAB$B_BID, DAB$B_BLN, DAB$B_VER_LETTER, DAB(100)   , 	INTEGER*2 DAB$W_MSG_BUF_LEN, DAB$W_MSG_LEN,& 	1	  DAB$W_AUX_BUF_LEN, DAB$W_AUX_LEN,. 	2	  DAB$W_IDI, DAB$W_STATE, DAB$W_REC_LENGTH,2 	3	  DAB$W_VERSION, DAB$W_LEVEL, DAB$W_BASE_LEVEL,) 	4	  DAB$W_UDK_INDEX, DAB$W_COLUMNS_PAGE, * 	5	  DAB$W_TT_CHANNEL, DAB$W_CTLC_CHANNEL,- 	6	  DAB$W_MAIN_WINDOW_X, DAB$W_MAIN_WINDOW_Y   ( 	INTEGER*4 DAB$A_MSG_BUF, DAB$A_AUX_BUF,1 	1	  DAB$L_CONDITION, DAB$L_FLAGS, DAB$L_OPTIONS, 0 	2	  DAB$L_KEYTABLE_ID, DAB$L_COMMAND_KEYBOARD, * 	3	  DAB$L_PROMPT_KEYBOARD, DAB$A_LOG_RAB,* 	4	  DAB$L_LMF_CONTEXT, DAB$L_RETURN_VALUE   	COMMON /DAB_COMMON/   	1	DAB$B_BID,  	2	DAB$B_BLN,  	3	DAB$L_CONDITION,  	4	DAB$A_MSG_BUF,  	5 	DAB$W_MSG_BUF_LEN,   	6	DAB$W_MSG_LEN,  	7	DAB$A_AUX_BUF,  	8	DAB$W_AUX_BUF_LEN,  	9	DAB$W_AUX_LEN,  	1	DAB$W_IDI,  	2	DAB$W_STATE,  	3	DAB$L_FLAGS,  	4	DAB$L_OPTIONS,  	5	DAB$W_REC_LENGTH,   	6	DAB$W_VERSION,  	7	DAB$W_LEVEL,  	8	DAB$B_VER_LETTER,   	9	DAB$W_BASE_LEVEL,   	1	DAB$W_UDK_INDEX,  	2	DAB$W_COLUMNS_PAGE, 	3	DAB$W_TT_CHANNEL, 	4	DAB$W_CTLC_CHANNEL, 	5	DAB$L_KEYTABLE_ID,  	6	DAB$L_COMMAND_KEYBOARD, 	7	DAB$L_PROMPT_KEYBOARD,  	8	DAB$A_LOG_RAB,  	9	DAB$L_LMF_CONTEXT,  	1	DAB$L_RETURN_VALUE, 	2	DAB$W_MAIN_WINDOW_X,  	3	DAB$W_MAIN_WINDOW_Y   	EQUIVALENCE (DAB, DAB$B_BID)     ; 	INTEGER    DTR$K_STL_CMD, DTR$K_STL_PRMPT, DTR$K_STL_LINE, 4 	1	   DTR$K_STL_MSG, DTR$K_STL_PGET, DTR$K_STL_PPUT,6 	2	   DTR$K_STL_CONT, DTR$K_STL_UDK, DTR$K_STL_END_UDK     	PARAMETER (DTR$K_STL_CMD=1,  	1	   DTR$K_STL_PRMPT=2,  	2	   DTR$K_STL_LINE=3,  	3	   DTR$K_STL_MSG=4, 	4	   DTR$K_STL_PGET=5,  	5	   DTR$K_STL_PPUT=6,  	6	   DTR$K_STL_CONT=7,  	7	   DTR$K_STL_UDK=8, 	8	   DTR$K_STL_END_UDK=9)  2 	INTEGER DTR$K_SEMI_COLON_OPT, DTR$K_UNQUOTED_LIT,+ 	1	DTR$K_SYNTAX_PROMPT, DTR$K_IMMED_RETURN, : 	2	DTR$K_FORMS_ENABLE, DTR$K_VERIFY, DTR$K_CONTEXT_SEARCH,; 	3	DTR$K_HYPHEN_DISABLED, DTR$K_MORE_COMMANDS, DTR$K_ABORT,  	4	DTR$K_LOCK_WAIT  # 	PARAMETER (DTR$K_SEMI_COLON_OPT=1,  	1	   DTR$K_UNQUOTED_LIT=16, 	2	   DTR$K_SYNTAX_PROMPT=32,  	3	   DTR$K_IMMED_RETURN=64, 	4	   DTR$K_FORMS_ENABLE=128,  	5	   DTR$K_VERIFY=256,   	6	   DTR$K_CONTEXT_SEARCH=2048,! 	7	   DTR$K_HYPHEN_DISABLED=4096,  	8	   DTR$K_MORE_COMMANDS=8192,  	9	   DTR$K_ABORT=16384, 	1	   DTR$K_LOCK_WAIT=32768)  = 	INTEGER	   DTR$M_OPT_CMD, DTR$M_OPT_PRMPT,   DTR$M_OPT_LINE, 4 	1	   DTR$M_OPT_MSG, DTR$M_OPT_PGET, DTR$M_OPT_PPUT,7 	2	   DTR$M_OPT_CONT, DTR$M_OPT_UDK, DTR$M_OPT�                                                                                                                                                                                                                                                                           5�X� $      
DTRJA061.C                     � -  
  [C]DAB.FOR;1                                                                                                                   A   � 	                         �o             _DTR_UDK, + 	3	   DTR$M_OPT_END_UDK, DTR$M_OPT_UNWIND,  . 	4	   DTR$M_OPT_CONTROL_C, DTR$M_OPT_STARTUP, A 	5	   DTR$M_OPT_FOREIGN, DTR$M_OPT_BANNER, DTR$M_OPT_REMOVE_CTLC,  	6	   DTR$M_OPT_KEYDEFS    	PARAMETER (DTR$M_OPT_CMD=1, 	1	   DTR$M_OPT_PRMPT=2, 	2	   DTR$M_OPT_LINE=4,  	3	   DTR$M_OPT_MSG=8, 	4	   DTR$M_OPT_PGET=16, 	5	   DTR$M_OPT_PPUT=32, 	6	   DTR$M_OPT_CONT=64, 	7	   DTR$M_OPT_UDK=128, 	8	   DTR$M_OPT_DTR_UDK=256, 	9	   DTR$M_OPT_END_UDK=512, 	1	   DTR$M_OPT_UNWIND=1024, 	2	   DTR$M_OPT_CONTROL_C=2048,  	3	   DTR$M_OPT_STARTUP=4096,  	4	   DTR$M_OPT_FOREIGN=8192,  	5	   DTR$M_OPT_BANNER=16384, " 	6	   DTR$M_OPT_REMOVE_CTLC=32768, 	7	   DTR$M_OPT_KEYDEFS=65536)  < 	INTEGER    DTR$K_UDK_SET, DTR$K_UDK_SET_NO, DTR$K_UDK_SHOW,- 	1	   DTR$K_UDK_STATEMENT,  DTR$K_UDK_COMMAND    	PARAMETER (DTR$K_UDK_SET=1,	  	1	   DTR$K_UDK_SET_NO=2,  	2	   DTR$K_UDK_SHOW=3,  	3	   DTR$K_UDK_STATEMENT=4, 	4	   DTR$K_UDK_COMMAND=5)  0 	INTEGER	   DTR$K_TOK_TOKEN, DTR$K_TOK_PICTURE, , 	1	   DTR$K_TOK_FILENAME, DTR$K_TOK_COMMAND,3 	2	   DTR$K_TOK_TEST_TOKEN, DTR$K_TOK_LIST_ELEMENT,  	3	   DTR$K_TOK_TEST_EOL   	PARAMETER (DTR$K_TOK_TOKEN=1, 	1	   DTR$K_TOK_PICTURE=2, 	2	   DTR$K_TOK_FILENAME=3,  	3	   DTR$K_TOK_COMMAND=4, 	4	   DTR$K_TOK_TEST_TOKEN=5,  	5	   DTR$K_TOK_LIST_ELEMENT=6,  	6	   DTR$K_TOK_TEST_EOL=7)      	CHARACTER*80  MSG_BUFF  	CHARACTER*20  AUX_BUFF & 	COMMON /DTR$BUFFERS/MSG_BUFF,AUX_BUFF   	EXTERNAL   DTR$_SUCCESS��                                                            �              
 * [C]DAB.H;1 +  , !8   .     / s P   4 M       �  �                - 
    0  � 1    2   3      K � P   W   O     5   6  &�K)��  7  �r)�*�  8          9 @����  G s P  H �� J �                     #ifndef DAB_H_DEFINED  /*   *	VAX C DATATRIEVE Access Block  *  *  USAGE NOTE  *M  *  If you want to use this DAB include file, with declarations coherent with L  *  documentation in "VAX DATATRIEVE Guide to Programming and Customizing", I  *  the typical variable declarations used in many of the examples can be   *  declared as follows:  *  *	#include <dtr$library/dab.h>   *  *	DTR_access_block	dab;  *	char			msg_buf[80];  *	char			aux_buf[80];  *  */  #include <descrip>   /*   *	Constant declarations section  */      /* Stallpoints */    #define DTR$K_STL_CMD		1 #define DTR$K_STL_PRMPT		2 #define DTR$K_STL_LINE		3  #define DTR$K_STL_MSG		4 #define DTR$K_STL_PGET		5  #define DTR$K_STL_PPUT		6  #define DTR$K_STL_CONT		7  #define DTR$K_STL_UDK		8 #define DTR$K_STL_END_UDK	9      /* Initialization Options */   #define DTR$K_SEMI_COLON_OPT	1 #define DTR$K_UNQUOTED_LIT	16  #define DTR$K_SYNTAX_PROMPT	32 #define DTR$K_IMMED_RETURN	64  #define DTR$K_FORMS_ENABLE	128 #define DTR$K_VERIFY		256 ! #define DTR$K_CONTEXT_SEARCH	2048 " #define DTR$K_HYPHEN_DISABLED	4096  #define DTR$K_MORE_COMMANDS	8192 #define DTR$K_ABORT		16384 #define DTR$K_LOCK_WAIT		32768     /* DTR$DTR call options */   #define DTR$M_OPT_CMD		1 #define DTR$M_OPT_PRMPT		2 #define DTR$M_OPT_LINE		4  #define DTR$M_OPT_MSG		8 #define DTR$M_OPT_PGET		16 #define DTR$M_OPT_PPUT		32 #define DTR$M_OPT_CONT		64 #define DTR$M_OPT_UDK		128 #define DTR$M_OPT_DTR_UDK	256  #define DTR$M_OPT_END_UDK	512  #define DTR$M_OPT_UNWIND	1024   #define DTR$M_OPT_CONTROL_C	2048 #define DTR$M_OPT_STARTUP	4096 #define DTR$M_OPT_FOREIGN	8192 #define DTR$M_OPT_BANNER	16384# #define DTR$M_OPT_REMOVE_CTLC	32768  #define DTR$M_OPT_KEYDEFS	65536       /* User-defined keyword types */   #define DTR$K_UDK_SET		1 #define DTR$K_UDK_SET_NO	2 #define DTR$K_UDK_SHOW		3  #define DTR$K_UDK_STATEMENT	4  #define DTR$K_UDK_COMMAND	5     & /* User-defined keyword token types */   #define DTR$K_TOK_TOKEN		1 #define DTR$K_TOK_PICTURE	2  #define DTR$K_TOK_FILENAME	3 #define DTR$K_TOK_COMMAND	4  #define DTR$K_TOK_TEST_TOKEN	5  #define DTR$K_TOK_LIST_ELEMENT	6 #define DTR$K_TOK_TEST_EOL	7     /* DTR Status messages */    #define DTR$_SUCCESS		9274723      /* DTR$INFO type constants */    #define DTR$K_INF_TYPE_DOMAIN	1 # #define DTR$K_INF_TYPE_COLLECTION 2   #define DTR$K_INF_TYPE_KEYWORD	3! #define DTR$K_INF_TYPE_DIC_NAME	4  #define DTR$K_INF_TYPE_GLV	5 #define DTR$K_INF_TYPE_PLOT	6      /* DTR$LOOKUP info constants */    #define DTR$K_INF_DOM_FLD	9  #define DTR$K_INF_DOM_FORM	10  #define DTR$K_INF_DOM_SHARE	11 #define DTR$K_INF_DOM_ACCESS	12  #define DTR$K_INF_DOM_NAME	13 ! #define DTR$K_INF_DOM_NEXT_DOM	14  #define DTR$K_INF_DOM_SSC	15  #define DTR$K_INF_DOM_REC_LEN	58   #define DTR$K_INF_FLD_NAME	16  #define DTR$K_INF_FLD_QNAME	17  #define DTR$K_INF_FLD_PICTURE	18 #define DTR$K_INF_FLD_EDIT	19  #define DTR$K_INF_FLD_DTYPE	20 #define DTR$K_INF_FLD_OFFSET	21  #define DTR$K_INF_FLD_LENGTH	22  #define DTR$K_INF_FLD_SCALE	23 #define DTR$K_INF_FLD_CHILD	24 #define DTR$K_INF_FLD_CNT	25 #define DTR$K_INF_FLD_LIST	26 " #define DTR$K_INF_FLD_REDEFINES	27  #define DTR$K_INF_FLD_VIRTUAL	28 #define DTR$K_INF_FLD_FILLER	29   #define DTR$K_INF_FLD_MISSING	30$ #define DTR$K_INF_FLD_MISSING_TXT 59# #define DTR$K_INF_FLD_SEG_STRING 60    #define DTR$K_INF_COL_CURSOR	1 #define DTR$K_INF_COL_SIZE	2 #define DTR$K_INF_COL_FLD	3  #define DTR$K_INF_COL_DROPPED	4  #define DTR$K_INF_COL_ERASED	5! #define DTR$K_INF_COL_INVISIBLE	6  #define DTR$K_INF_COL_NAME	7  #define DTR$K_INF_COL_NEXT_COL	8  " #define DTR$K_INF_GLV_FIRST_DOM	34" #define DTR$K_INF_GLV_FIRST_COL	35" #define DTR$K_INF_GLV_FIRST_SSC	36  #define DTR$K_INF_GLV_DEF_DIC	61   #define DTR$K_INF_FRM_NAME	32   #define DTR$K_INF_FRM_LIBRARY	33   #define DTR$K_INF_SSC_NAME	46  #define DTR$K_INF_SSC_SET	47 #define DTR$K_INF_SSC_NEXT	48    #define DTR$K_INF_SET_NAME	49  #define DTR$K_INF_SET_NEXT	50  #define DTR$K_INF_SET_SDP	51! #define DTR$K_INF_SET_SINGULAR	52    #define DTR$K_INF_SDP_NEXT	53  #define DTR$K_INF_SDP_DOMAIN	54   #define DTR$K_INF_SDP_TENANCY	55 #define DTR$K_INF_SDP_INSERT	56  #define DTR$K_INF_SDP_RETAIN	57    #define DTR$K_INF_FLD_QHDR	31    #define DTR$K_INF_HDR_CNT	40 #define DTR$K_INF_HDR_STRING	41     #define DTR$K_INF_GLV_STA_OBJ	37  #define DTR$K_INF_GLV_STA_CNT	38! #define DTR$K_INF_GLV_STA_LINE	39    #define DTR$K_INF_PLO_CNT	42 #define DTR$K_INF_PLO_PAI	43   #define DTR$K_INF_PAI_PROMPT	44  #define DTR$K_INF_PAI_DTYPE	45  ! /* DTR$LOOKUP answer constants */   # #define DTR$K_INF_DOM_ACCESS_READ	1 $ #define DTR$K_INF_DOM_ACCESS_WRITE	2% #define DTR$K_INF_DOM_ACCESS_MODIFY	3 % #define DTR$K_INF_DOM_ACCESS_EXTEND	4   ' #define DTR$K_INF_DOM_SHARE_EXCLUSIVE	1 $ #define DTR$K_INF_DOM_SHARE_SHARED	2% #define DTR$K_INF_DOM_SHARE_PROTECT	3    /*F  * On AXP platform, we make sure that member alignment is kept in line$  * with other languages for the DAB   */ 
 #if __DECC #pragma member_alignment save  #pragma nomember_alignment #endif   /*  *	DAB  */  typedef struct { 	char		dab$b_bid;  	char		dab$b_bln;  	long		dab$l_condition;    	char 		*dab$a_msg_buf; " 	unsigned short	dab$w_msg_buf_len; 	unsigned short	dab$w_msg_len;   	char		*dab$a_aux_buf;" 	unsigned short	dab$w_aux_buf_len; 	unsigned short	dab$w_aux_len;   	unsigned short	dab$w_idi; 	unsigned short	dab$w_state; 	unsigned long	dab$l_flags;  	unsigned long	dab$l_options;   ! 	unsigned short	dab$w_rec_length;  	unsigned short	dab$w_version; 	unsigned short	dab$w_level; 	char		dab$b_ver_letter;! 	unsigned short	dab$w_base_level;  	short		dab$w_udk_index;# 	unsigned short	dab$w_columns_page; ! 	unsigned short	dab$w_tt_channel; # 	unsigned short	dab$w_ctlc_channel;  	long		dab$l_keytable_id;  	long		dab$l_command_keyboard; 	long		dab$l_prompt_keyboard;  	unsigned long	dab$a_log_rab; ! 	unsigned long	dab$l_lmf_context; # 	unsigned long	dab$l_return_value;	  	short		dab$w_main_window_x; 	short		dab$w_main_window_y; 	char		dab$rest_of_dab[21];  } DTR_access_block;   
 #if __DECC  #pragma member_alignment restore #endif     /*"  *	External procedure declarations  */    extern long 3 dtr$command	(DTR_access_block *,       	/* dab			*/ 1 		struct dsc$descriptor_s *,	/* command_string	*/  		...);				/* [p1],...[pn]		*/ extern long / dtr$continue	(DTR_access_block *);		/* dab			*/    extern long 0 dtr$create_udk	(DTR_access_block *,		/* dab			*/. 		struct dsc$descriptor_s *,	/* udk string		*/ 		short *,			/* udk index		*/   		short *);			/* udk context		*/   extern long * dtr$�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          eS�f $      
DTRJA061.C                     !8  
  
[C]DAB.H;1                                                                                                                     M   �                          gU             dtr 	(DTR_access_block *,		/* dab			*/  		long *);			/* options code		*/   extern long . dtr$end_udk	(DTR_access_block *);		/* dab			*/   extern long - dtr$finish	(DTR_access_block *);		/* dab			*/    extern long 4 dtr$finish_windows (DTR_access_block *);	/* dab			*/   extern long . dtr$get_port	(DTR_access_block *,		/* dab			*/  		char *);			/* record buffer	*/   extern long 0 dtr$get_string	(DTR_access_block *,		/* dab			*/ 		long *,				/* token type		*/ 		...);				/* [string] 						   [string length] 						   [compare string]	*/   extern long * dtr$info	(DTR_access_block *,		/* dab			*/ 		long *,				/* object id		*/  		char *,				/* info code		*/   		long *,				/* return value		*/ 		...);				/* [output string]  						   [index]		*/   extern long * dtr$init	(DTR_access_block *,		/* dab			*/ 		...);				/* [size of stack]  						   [message buffer]  						   [aux buffer]  						   [options code]	*/   extern long , dtr$lookup	(DTR_access_block *,		/* dab			*/ 		char *,				/* object type		*/  		long *,				/* object id		*/  		...);				/* [object name]	*/   extern long / dtr$port_eof	(DTR_access_block *);		/* dab			*/    extern long 0 dtr$print_dab	(DTR_access_block *);		/* dab			*/   extern long 0 dtr$put_output	(DTR_access_block *,		/* dab			*/* 		struct dsc$descriptor_s *,	/* string		*/  		...);				/* [prompt string]	*/   extern long . dtr$put_port	(DTR_access_block *,		/* dab			*/  		char *);			/* record buffer	*/   extern long / dtr$put_value	(DTR_access_block *,		/* dab			*/  		...);				/* [value]		*/    extern long - dtr$unwind	(DTR_access_block *);		/* dab			*/    extern long 0 dtr$window_msg	(DTR_access_block *,		/* dab			*/2 		struct dsc$descriptor_s *);	/* message string	*/   extern long 3 dtr$window_output (DTR_access_block *,		/* dab			*/ 0 		struct dsc$descriptor_s *);	/* text string		*/   extern long . dtr$windows 	(DTR_access_block *,		/* dab			*/  		long *);			/* options code		*/   #define DAB_H_DEFINED  #endif��                                                      �               * [C]DAB.LIB;1 +  , _   . 	    / s P   4 =   	   	    �                - 
    0  � 1    2   3      K � P   W   O 
    5   6 � �  7 `�)�*�  8          9 @����  G s P  H �� J �                   * COBOL DATATRIEVE Access Block    01	DAB.  	03 BLOCK_ID.  		05 DAB$B_BID 	     PIC X.  		05 DAB$B_BLN 	     PIC X. ) 	03 DAB$L_CONDITION 	     PIC S9(9) COMP.          03 MSG_BUFFER.3                 05 DAB$A_MSG_BUF     PIC 9(9) COMP. 3                 05 DAB$W_MSG_BUF_LEN PIC 9(4) COMP. 3                 05 DAB$W_MSG_LEN     PIC 9(4) COMP.          03 AUX_BUFFER.3                 05 DAB$A_AUX_BUF     PIC 9(9) COMP. 3                 05 DAB$W_AUX_BUF_LEN PIC 9(4) COMP. 3                 05 DAB$W_AUX_LEN     PIC 9(4) COMP. " 	03 DAB$W_IDI		     PIC 9(4) COMP.$ 	03 DAB$W_STATE		     PIC 9(4) COMP.& 	   88 DTR$K_STL_CMD          VALUE 1.& 	   88 DTR$K_STL_PRMPT        VALUE 2.& 	   88 DTR$K_STL_LINE         VALUE 3.& 	   88 DTR$K_STL_MSG          VALUE 4.& 	   88 DTR$K_STL_PGET         VALUE 5.& 	   88 DTR$K_STL_PPUT         VALUE 6.& 	   88 DTR$K_STL_CONT         VALUE 7.& 	   88 DTR$K_STL_UDK          VALUE 8.& 	   88 DTR$K_STL_END_UDK      VALUE 9.& 	03 DAB$L_FLAGS   	     PIC 9(9) COMP.& 	03 DAB$L_OPTIONS 	     PIC 9(9) COMP.& 	03 DAB$W_REC_LEN 	     PIC 9(4) COMP.& 	03 DAB$W_VERSION 	     PIC 9(4) COMP.& 	03 DAB$W_LEVEL   	     PIC 9(4) COMP.! 	03 DAB$B_VER_LETTER 	     PIC X. ( 	03 DAB$W_BASE_LEVEL	     PIC 9(4) COMP.' 	03 DAB$W_UDK_INDEX	     PIC 9(4) COMP. * 	03 DAB$W_COLUMNS_PAGE	     PIC 9(4) COMP., 	03 DAB$W_TT_CHANNEL          PIC 9(4) COMP., 	03 DAB$W_CTLC_CHANNEL        PIC 9(4) COMP., 	03 DAB$L_KEYTABLE_ID         PIC 9(9) COMP., 	03 DAB$L_COMMAND_KEYBOARD    PIC 9(9) COMP., 	03 DAB$L_PROMPT_KEYBOARD     PIC 9(9) COMP.% 	03 DAB$A_LOG_RAB	     PIC 9(9) COMP. ) 	03 DAB$L_LMF_CONTEXT	     PIC 9(9) COMP. * 	03 DAB$L_RETURN_VALUE	     PIC 9(9) COMP.+ 	03 DAB$W_MAIN_WINDOW_X	     PIC 9(4) COMP. + 	03 DAB$W_MAIN_WINDOW_Y	     PIC 9(4) COMP. $ 	03 DAB$REST_OF_DAB 	     PIC X(21).   01 INITIAL_OPTIONS. 2     03 DTR$K_SEMI_COLON_OPT	PIC 9(9) COMP VALUE 1.1     03 DTR$K_UNQUOTED_LIT	PIC 9(9) COMP VALUE 16. 2     03 DTR$K_SYNTAX_PROMPT	PIC 9(9) COMP VALUE 32.1     03 DTR$K_IMMED_RETURN	PIC 9(9) COMP VALUE 64. 2     03 DTR$K_FORMS_ENABLE	PIC 9(9) COMP VALUE 128.-     03 DTR$K_VERIFY		PIC 9(9) COMP VALUE 256. 5     03 DTR$K_CONTEXT_SEARCH	PIC 9(9) COMP VALUE 2048. 6     03 DTR$K_HYPHEN_DISABLED	PIC 9(9) COMP VALUE 4096.4     03 DTR$K_MORE_COMMANDS	PIC 9(9) COMP VALUE 8192..     03 DTR$K_ABORT		PIC 9(9) COMP VALUE 16384.:     03 DTR$K_LOCK_WAIT          PIC 9(9) COMP VALUE 32768.   01 DTR$DTR_OPTIONS. -     03 DTR$M_OPT_CMD		PIC 9(9) COMP VALUE  1. /     03 DTR$M_OPT_PRMPT		PIC 9(9) COMP VALUE  2. .     03 DTR$M_OPT_LINE		PIC 9(9) COMP VALUE  4.-     03 DTR$M_OPT_MSG		PIC 9(9) COMP VALUE  8. /     03 DTR$M_OPT_PGET		PIC 9(9) COMP VALUE  16. /     03 DTR$M_OPT_PPUT		PIC 9(9) COMP VALUE  32. /     03 DTR$M_OPT_CONT		PIC 9(9) COMP VALUE  64. /     03 DTR$M_OPT_UDK		PIC 9(9) COMP VALUE  128. 2     03 DTR$M_OPT_DTR_UDK	PIC 9(9) COMP VALUE  256.2     03 DTR$M_OPT_END_UDK	PIC 9(9) COMP VALUE  512.3     03 DTR$M_OPT_UNWIND		PIC 9(9) COMP VALUE  1024. 5     03 DTR$M_OPT_CONTROL_C	PIC 9(9) COMP VALUE  2048. 3     03 DTR$M_OPT_STARTUP	PIC 9(9) COMP VALUE  4096. 3     03 DTR$M_OPT_FOREIGN	PIC 9(9) COMP VALUE  8192. 4     03 DTR$M_OPT_BANNER		PIC 9(9) COMP VALUE  16384.8     03 DTR$M_OPT_REMOVE_CTLC	PIC 9(9) COMP VALUE  32768.4     03 DTR$M_OPT_KEYDEFS	PIC 9(9) COMP VALUE  65536.   01 DTR$UDK_CONTEXTS.,     03 DTR$K_UDK_SET		PIC 9(4) COMP VALUE 1./     03 DTR$K_UDK_SET_NO 	PIC 9(4) COMP VALUE 2. -     03 DTR$K_UDK_SHOW		PIC 9(4) COMP VALUE 3. 1     03 DTR$K_UDK_STATEMENT	PIC 9(4) COMP VALUE 4. /     03 DTR$K_UDK_COMMAND	PIC 9(4) COMP VALUE 5.    01 DTR$UDK_TOKENS..     03 DTR$K_TOK_TOKEN		PIC 9(4) COMP VALUE 1./     03 DTR$K_TOK_PICTURE	PIC 9(4) COMP VALUE 2. 0     03 DTR$K_TOK_FILENAME	PIC 9(4) COMP VALUE 3./     03 DTR$K_TOK_COMMAND	PIC 9(4) COMP VALUE 4. 2     03 DTR$K_TOK_TEST_TOKEN	PIC 9(4) COMP VALUE 5.4     03 DTR$K_TOK_LIST_ELEMENT	PIC 9(4) COMP VALUE 6.0     03 DTR$K_TOK_TEST_EOL	PIC 9(4) COMP VALUE 7.   01 MSG_BUFF			PIC X(80). 01 AUX_BUFF			PIC X(20).  = 01 DTR$_SUCCESS PIC 9(9) COMP VALUE IS EXTERNAL DTR$_SUCCESS.   ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      �               * [C]DAB.PAS;1 +  , i   .     / s P   4 O       �   �                - 
    0  � 1    2   3      K � P   W   O     5   6 `�1)��  7 @��)�*�  8          9 @����  G s P  H �� J �                  $ (* Pascal DATATRIEVE Access Block *)  # (* Constant declarations section *)    CONST    (* Stallpoints *)        DTR$K_STL_CMD=1;     DTR$K_STL_PRMPT=2;     DTR$K_STL_LINE=3;      DTR$K_STL_MSG=4;     DTR$K_STL_PGET=5;      DTR$K_STL_PPUT=6;      DTR$K_STL_CONT=7;      DTR$K_STL_UDK=8;     DTR$K_STL_END_UDK=9;   (* Initialization Options *)       DTR$K_SEMI_COLON_OPT=1;      DTR$K_UNQUOTED_LIT=16;     DTR$K_SYNTAX_PROMPT=32;      DTR$K_IMMED_RETURN=64;     DTR$K_FORMS_ENABLE=128;      DTR$K_VERIFY=256;      DTR$K_CONTEXT_SEARCH=2048;     DTR$K_HYPHEN_DISABLED=4096;      DTR$K_MORE_COMMANDS=8192;      DTR$K_ABORT=16384;     DTR$K_LOCK_WAIT=32768;   (* DTR$DTR call options *)       DTR$M_OPT_CMD=1;     DTR$M_OPT_PRMPT=2;     DTR$M_OPT_LINE=4;      DTR$M_OPT_MSG=8;     DTR$M_OPT_PGET=16;     DTR$M_OPT_PPUT=32;     DTR$M_OPT_CONT=64;     DTR$M_OPT_UDK=128;     DTR$M_OPT_DTR_UDK=256;     DTR$M_OPT_END_UDK=512;     DTR$M_OPT_UNWIND=1024;     DTR$M_OPT_CONTROL_C=2048;      DTR$M_OPT_STARTUP=4096;      DTR$M_OPT_FOREIGN=8192;      DTR$M_OPT_BANNER=16384;       DTR$M_OPT_REMOVE_CTLC=32768;     DTR$M_OPT_KEYDEFS=65536;    (* User-defined keyword types *)       DTR$K_UDK_SET=1;     DTR$K_UDK_SET_NO=2;      DTR$K_UDK_SHOW=3;      DTR$K_UDK_STATEMENT=4;     DTR$K_UDK_COMMAND=5;  & (* User-defined keyword token types *)       DTR$K_TOK_TOKEN=1;     DTR$K_TOK_PICTURE=2;     DTR$K_TOK_FILENAME=3;      DTR$K_TOK_COMMAND=4;     DTR$K_TOK_TEST_TOKEN=5;      DTR$K_TOK_LIST_ELEME�                                                                                                                                                                                                                                                                           �y� $      
DTRJA061.C                     i  
  [C]DAB.PAS;1                                                                                                                   O   �                          �&              NT=6;      DTR$K_TOK_TEST_EOL=7;    (* DTR Status messages *)        DTR$_SUCCESS=9274723;    (* DTR$INFO type constants *)      DTR$K_INF_TYPE_DOMAIN=1;      DTR$K_INF_TYPE_COLLECTION=2;     DTR$K_INF_TYPE_KEYWORD=3;      DTR$K_INF_TYPE_DIC_NAME=4;     DTR$K_INF_TYPE_GLV=5;      DTR$K_INF_TYPE_PLOT=6;   (* DTR$LOOKUP info constants *)      DTR$K_INF_DOM_FLD=9;     DTR$K_INF_DOM_FORM=10;     DTR$K_INF_DOM_SHARE=11;      DTR$K_INF_DOM_ACCESS=12;     DTR$K_INF_DOM_NAME=13;     DTR$K_INF_DOM_NEXT_DOM=14;     DTR$K_INF_DOM_SSC=15;      DTR$K_INF_FLD_NAME=16;     DTR$K_INF_FLD_QNAME=17;      DTR$K_INF_FLD_PICTURE=18;      DTR$K_INF_FLD_EDIT=19;     DTR$K_INF_FLD_DTYPE=20;      DTR$K_INF_FLD_OFFSET=21;     DTR$K_INF_FLD_LENGTH=22;     DTR$K_INF_FLD_SCALE=23;      DTR$K_INF_FLD_CHILD=24;      DTR$K_INF_FLD_CNT=25;      DTR$K_INF_FLD_LIST=26;     DTR$K_INF_FLD_REDEFINES=27;      DTR$K_INF_FLD_VIRTUAL=28;      DTR$K_INF_FLD_FILLER=29;     DTR$K_INF_FLD_MISSING=30;      DTR$K_INF_COL_CURSOR=1;      DTR$K_INF_COL_SIZE=2;      DTR$K_INF_COL_FLD=3;     DTR$K_INF_COL_DROPPED=4;     DTR$K_INF_COL_ERASED=5;      DTR$K_INF_COL_INVISIBLE=6;     DTR$K_INF_COL_NAME=7;      DTR$K_INF_COL_NEXT_COL=8;      DTR$K_INF_GLV_FIRST_DOM=34;      DTR$K_INF_GLV_FIRST_COL=35;      DTR$K_INF_GLV_FIRST_SSC=36;      DTR$K_INF_FRM_NAME=32;     DTR$K_INF_FRM_LIBRARY=33;      DTR$K_INF_SSC_NAME=46;     DTR$K_INF_SSC_SET=47;      DTR$K_INF_SSC_NEXT=48;     DTR$K_INF_SET_NAME=49;     DTR$K_INF_SET_NEXT=50;     DTR$K_INF_SET_SDP=51;      DTR$K_INF_SET_SINGULAR=52;     DTR$K_INF_SDP_NEXT=53;     DTR$K_INF_SDP_DOMAIN=54;     DTR$K_INF_SDP_TENANCY=55;      DTR$K_INF_SDP_INSERT=56;     DTR$K_INF_SDP_RETAIN=57;     DTR$K_INF_FLD_QHDR=31;     DTR$K_INF_HDR_CNT=40;      DTR$K_INF_HDR_STRING=41;     DTR$K_INF_GLV_STA_OBJ=37;      DTR$K_INF_GLV_STA_CNT=38;      DTR$K_INF_GLV_STA_LINE=39;     DTR$K_INF_PLO_CNT=42;      DTR$K_INF_PLO_PAI=43;      DTR$K_INF_PAI_PROMPT=44;     DTR$K_INF_PAI_DTYPE=45;      DTR$K_INF_DOM_REC_LEN=58;   ! (* DTR$LOOKUP answer constants *)       DTR$K_INF_DOM_ACCESS_READ=1;!     DTR$K_INF_DOM_ACCESS_WRITE=2; "     DTR$K_INF_DOM_ACCESS_MODIFY=3;"     DTR$K_INF_DOM_ACCESS_EXTEND=4;$     DTR$K_INF_DOM_SHARE_EXCLUSIVE=1;!     DTR$K_INF_DOM_SHARE_SHARED=2; "     DTR$K_INF_DOM_SHARE_PROTECT=3;   (* Type declarations section *)    TYPE       DTR$BYTE = [BYTE] 0..255;      DTR$WORD = [WORD] 0..65535; +     DTR$SIGNED_WORD = [WORD] -32768..32767;   	 (* Dab *)   $     DTR_access_block = packed record 	DAB$B_BID 	  	: DTR$BYTE; 	DAB$B_BLN 	  	: DTR$BYTE; 	DAB$L_CONDITION   	: INTEGER; 	DAB$A_MSG_BUF 		: UNSIGNED; 	DAB$W_MSG_BUF_LEN 	: DTR$WORD;  	DAB$W_MSG_LEN 		: DTR$WORD; 	DAB$A_AUX_BUF 		: UNSIGNED; 	DAB$W_AUX_BUF_LEN 	: DTR$WORD;  	DAB$W_AUX_LEN 		: DTR$WORD; 	DAB$W_IDI 	  	: DTR$WORD; 	DAB$W_STATE 		: DTR$WORD; 	DAB$L_FLAGS 		: UNSIGNED; 	DAB$L_OPTIONS 		: UNSIGNED; 	DAB$W_REC_LENGTH	: DTR$WORD;  	DAB$W_VERSION 		: DTR$WORD; 	DAB$W_LEVEL		: DTR$WORD;  	DAB$B_VER_LETTER  	: DTR$BYTE;  	DAB$W_BASE_LEVEL  	: DTR$WORD; & 	DAB$W_UDK_INDEX   	: DTR$SIGNED_WORD;  	DAB$W_COLUMNS_PAGE 	: DTR$WORD; 	DAB$W_TT_CHANNEL  	: DTR$WORD;   	DAB$W_CTLC_CHANNEL 	: DTR$WORD; 	DAB$L_KEYTABLE_ID 	: UNSIGNED; $ 	DAB$L_COMMAND_KEYBOARD  : UNSIGNED;# 	DAB$L_PROMPT_KEYBOARD 	: UNSIGNED;  	DAB$A_LOG_RAB		: UNSIGNED;  	DAB$L_LMF_CONTEXT	: UNSIGNED; 	DAB$L_RETURN_VALUE	: UNSIGNED;   	DAB$W_MAIN_WINDOW_X	: DTR$WORD;  	DAB$W_MAIN_WINDOW_Y	: DTR$WORD;?         DAB$REST_OF_DAB         : PACKED ARRAY [1..21] OF CHAR;      end;  0     DTR$REC_TYPE = PACKED ARRAY [1..80] OF CHAR;  # (* Variable declarations section *)  VAR	 	DAB 		: DTR_access_block;5 	MSG_BUFF 	: [volatile] PACKED ARRAY [1..80] OF CHAR; 5 	AUX_BUFF 	: [volatile] PACKED ARRAY [1..20] OF CHAR;   $ (* external function declarations *)   function DTR$COMMAND ( 	var dab : dtr_access_block;C 	command_string: [class_s] packed array [l1..u1 : integer] of char; > 	param : [class_s,unsafe,list] packed array [l2..u2 : integer]	 		of char  	) : integer; external;   I function DTR$CONTINUE ( var dab : dtr_access_block ) : integer; external;    function DTR$CREATE_UDK (  	var dab : dtr_access_block;B 	keyword_name : [class_s] packed array [l1..u1 : integer] of char; 	udk_index : DTR$WORD; 	context : DTR$WORD  	) : integer; external;    function DTR$DTR ( 	var dab : dtr_access_block; 	options_code : unsigned 	) : integer; external;   H function DTR$END_UDK ( var dab : dtr_access_block ) : integer; external;  G function DTR$FINISH ( var dab : dtr_access_block ) : integer; external;   O function DTR$FINISH_WINDOWS ( var dab : dtr_access_block ) : integer; external;    function DTR$GET_PORT (  	var dab : dtr_access_block;B 	%ref rec_buff : [unsafe] packed array [l1..u1 : integer] of char  	) : integer; external;    function DTR$GET_STRING (  	var dab : dtr_access_block; 	token_type : unsigned; @ 	var string : [truncate,class_s] packed array [l1..u1 : integer] 		 of char := %immed 0; . 	var length : [truncate] DTR$WORD := %immed 0;F 	compare_string : [class_s,truncate] packed array [l2..u2:integer] of  	char :=%immed 0 	) : integer; external;    function DTR$INFO (  	var dab : dtr_access_block; 	object_id : unsigned; 	info_code : DTR$BYTE; 	var ret_val : unsigned;D 	var output_string : [class_s,truncate] packed array [a..b:integer]  		of char := %immed 0;/ 	%immed index : [truncate] integer := %immed 0   	) : integer; external;    function DTR$INIT (  	var dab : dtr_access_block; 	size : unsigned := %immed 0; G 	var msg_buff:[truncate,class_s,volatile]packed array [l1..u1:integer]   		of char:= %immed 0; G 	var aux_buff:[truncate,class_s,volatile]packed array [l2..u2:integer]   		of char:= %immed 0; $ 	options_code : unsigned := %immed 0 	) : integer; external;    function DTR$LOOKUP (  	var dab : dtr_access_block; 	object_type : DTR$BYTE ;  	var object_id : unsigned;C 	object_name : [class_s,truncate] packed array [l1..u1:integer] of   		char := %immed 0 	) : integer; external;   H function DTR$PORT_EOF ( var dab : dtr_access_block ): integer; external;  J function DTR$PRINT_DAB ( var dab : dtr_access_block ) : integer; external;   function DTR$PUT_OUTPUT (  	var dab : dtr_access_block;< 	string : [class_s] packed array [l1..u1 : integer] of char;C 	prompt_string : [class_s] packed array [l2..u2 : integer] of char   		:= %immed 0  	) : integer; external;    function DTR$PUT_PORT (  	var dab : dtr_access_block;D 	%ref rec_buffer : [unsafe] packed array [l1..u1 : integer] of char  	) : integer; external;    function DTR$PUT_VALUE ( 	var dab : dtr_access_block;3 	value : [class_s] packed array [l1..u1 : integer]   		of char := %immed 0  	) : integer; external;   I function DTR$UNWIND   ( var dab : dtr_access_block ) : integer; external;    function DTR$WINDOW_MSG (  	var dab : dtr_access_block;; 	message_string : [class_s] packed array [l1..u1 : integer] 
 		of char  	) : integer; external;    function DTR$WINDOW_OUTPUT ( 	var dab : dtr_access_block;8 	text_string : [class_s] packed array [l1..u1 : integer]	 	of char   	) : integer; external;    function DTR$WINDOWS (   	var dab : dtr_access_block; 	options_code : unsigned 	) : integer; external;   O (* Sample declarations of parameters to the external procedures declared above:    	REC_BUFF	: DTR$REC_TYPE; % 	COMMAND_LINE	: VARYING [80] OF CHAR; % 	TEXT_STRING 	: VARYING [80] OF CHAR; $ 	TEST_TOKEN 	: VARYING [80] OF CHAR;! 	PROMPT 		: VARYING [80] OF CHAR;  	STK_SIZE 	: UNSIGNED; 	INIT_OPTIONS 	: UNSIGNED; 	DTR_OPTIONS 	: UNSIGNED;  	DTR_STATUS	: UNSIGNED;  	TOKEN_LENGTH 	: UNSIGNED; 	INFO_INDEX 	: UNSIGNED;   	UDK_INDEX 	: DTR$WORD;  	CONTEXT 	: DTR$WORD; & 	TOKEN_TYPE  	: UNSIGNED;					      *)    ��                                                                                                                                                                                                                                                                                                                                                        �               * [C]DAB.PLI;1 +  , ~   .     / s P   4 O       n  �                - 
    0  � 1    2   3      K � P   W   O     5   6  ?�J�;�  7 �j�)�*�  8          9 @����  G s P  H �� J �     �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          j��? $      
DTRJA061.C                     ~  
  [C]DAB.PLI;1                                                                                                                   O   �                          	� "            / /* PL/I DATATRIEVE Access Block declarations */ # /* Last revision: 07-Jan-1986 KG */    /*  *  PL/I Usage notes  *L  *  The initialization options, which are documented as DTR$K_xxx constants,J  *  are named DTR$M_xxx for PL/I.  Both the initialization options and theL  *  DTR$DTR options are represented as bit strings, and thus can be combined3  *  with the bitwise or (|) operator.  For example:   *<  *	STATUS = DTR$DTR( DAB, DTR$M_OPT_CMD | DTR$M_OPT_PRMPT );  *L  *  The entry point definitions use several features that are only availableJ  *  with VAX PL/I V3.0 and later.  (In particular, the OPTIONAL, TRUNCATE,  *  LIST and ANY DESCRIPTOR.)   *O  *  If you want the typical variable declarations used in many of the examples, $  *  they can be declared as follows:  *$  *  DCL 1 DAB LIKE DTR_ACCESS_BLOCK;  *  DCL MSG_BUF CHAR(80);   *  DCL AUX_BUF CHAR(20);   *L  *  If for some reason, you want to use your own declaration of the DAB, butK  *  still want to use the entry point definitions in this file, you can use N  *  the REFERENCE built-in function to override the declaration.  For example:  *  *  %INCLUDE 'DTR$LIBRARY:DAB';   *  DCL 1 MY_DAB, 
  *	  2 ...E  *  STATUS = DTR$DTR( REF(MY_DAB), DTR$M_OPT_CMD | DTR$M_OPT_PRMPT );   *M  *  Note that the ANY CHARACTER(*) parameter type can accept either CHARACTER B  *  or CHARACTER VARYING strings without creating dummy arguments.  */   # /* Constant declarations section */    /* Stallpoints */  %replace DTR$K_STL_CMD by 1; %replace DTR$K_STL_PRMPT by 2; %replace DTR$K_STL_LINE by 3;  %replace DTR$K_STL_MSG by 4; %replace DTR$K_STL_PGET by 5;  %replace DTR$K_STL_PPUT by 6;  %replace DTR$K_STL_CONT by 7;  %replace DTR$K_STL_UDK by 8;  %replace DTR$K_STL_END_UDK by 9;   /* Initialization Options */E %replace DTR$M_SEMI_COLON_OPT by '10000000000000000000000000000000'b; D %replace DTR$M_UNQUOTED_LIT by  '00001000000000000000000000000000'b;D %replace DTR$M_SYNTAX_PROMPT by '00000100000000000000000000000000'b;C %replace DTR$M_IMMED_RETURN by '00000010000000000000000000000000'b; C %replace DTR$M_FORMS_ENABLE by '00000001000000000000000000000000'b; = %replace DTR$M_VERIFY by '00000000100000000000000000000000'b; E %replace DTR$M_CONTEXT_SEARCH by '00000000000100000000000000000000'b; F %replace DTR$M_HYPHEN_DISABLED by '00000000000010000000000000000000'b;D %replace DTR$M_MORE_COMMANDS by '00000000000001000000000000000000'b;< %replace DTR$M_ABORT by '00000000000000100000000000000000'b;@ %replace DTR$M_LOCK_WAIT by '00000000000000010000000000000000'b;   /* DTR$DTR call options */> %replace DTR$M_OPT_CMD by '10000000000000000000000000000000'b;@ %replace DTR$M_OPT_PRMPT by '01000000000000000000000000000000'b;? %replace DTR$M_OPT_LINE by '00100000000000000000000000000000'b; > %replace DTR$M_OPT_MSG by '00010000000000000000000000000000'b;? %replace DTR$M_OPT_PGET by '00001000000000000000000000000000'b; ? %replace DTR$M_OPT_PPUT by '00000100000000000000000000000000'b; ? %replace DTR$M_OPT_CONT by '00000010000000000000000000000000'b; > %replace DTR$M_OPT_UDK by '00000001000000000000000000000000'b;B %replace DTR$M_OPT_DTR_UDK by '00000000100000000000000000000000'b;B %replace DTR$M_OPT_END_UDK by '00000000010000000000000000000000'b;A %replace DTR$M_OPT_UNWIND by '00000000001000000000000000000000'b; D %replace DTR$M_OPT_CONTROL_C by '00000000000100000000000000000000'b;B %replace DTR$M_OPT_STARTUP by '00000000000010000000000000000000'b;B %replace DTR$M_OPT_FOREIGN by '00000000000001000000000000000000'b;A %replace DTR$M_OPT_BANNER by '00000000000000100000000000000000'b; F %replace DTR$M_OPT_REMOVE_CTLC by '00000000000000010000000000000000'b;B %replace DTR$M_OPT_KEYDEFS by '00000000000000001000000000000000'b;      /* User-defined keyword types */ %replace DTR$K_UDK_SET by 1; %replace DTR$K_UDK_SET_NO by 2;  %replace DTR$K_UDK_SHOW by 3; " %replace DTR$K_UDK_STATEMENT by 4;  %replace DTR$K_UDK_COMMAND by 5;  & /* User-defined keyword token types */ %replace DTR$K_TOK_TOKEN by 1;  %replace DTR$K_TOK_PICTURE by 2;! %replace DTR$K_TOK_FILENAME by 3;   %replace DTR$K_TOK_COMMAND by 4;# %replace DTR$K_TOK_TEST_TOKEN by 5; % %replace DTR$K_TOK_LIST_ELEMENT by 6; ! %replace DTR$K_TOK_TEST_EOL by 7;    /* DTR Status messages */ ! %replace DTR$_SUCCESS by 9274723;    /* DTR$INFO type constants */ $ %replace DTR$K_INF_TYPE_DOMAIN by 1;( %replace DTR$K_INF_TYPE_COLLECTION by 2;% %replace DTR$K_INF_TYPE_KEYWORD by 3; & %replace DTR$K_INF_TYPE_DIC_NAME by 4;! %replace DTR$K_INF_TYPE_GLV by 5; " %replace DTR$K_INF_TYPE_PLOT by 6;   /* DTR$LOOKUP info constants */ # %replace DTR$K_INF_COL_CURSOR by 1; ! %replace DTR$K_INF_COL_SIZE by 2;   %replace DTR$K_INF_COL_FLD by 3;$ %replace DTR$K_INF_COL_DROPPED by 4;# %replace DTR$K_INF_COL_ERASED by 5; & %replace DTR$K_INF_COL_INVISIBLE by 6;! %replace DTR$K_INF_COL_NAME by 7; % %replace DTR$K_INF_COL_NEXT_COL by 8;   %replace DTR$K_INF_DOM_FLD by 9;" %replace DTR$K_INF_DOM_FORM by 10;# %replace DTR$K_INF_DOM_SHARE by 11; $ %replace DTR$K_INF_DOM_ACCESS by 12;" %replace DTR$K_INF_DOM_NAME by 13;& %replace DTR$K_INF_DOM_NEXT_DOM by 14;! %replace DTR$K_INF_DOM_SSC by 15; " %replace DTR$K_INF_FLD_NAME by 16;# %replace DTR$K_INF_FLD_QNAME by 17; % %replace DTR$K_INF_FLD_PICTURE by 18; " %replace DTR$K_INF_FLD_EDIT by 19;# %replace DTR$K_INF_FLD_DTYPE by 20; $ %replace DTR$K_INF_FLD_OFFSET by 21;$ %replace DTR$K_INF_FLD_LENGTH by 22;# %replace DTR$K_INF_FLD_SCALE by 23; # %replace DTR$K_INF_FLD_CHILD by 24; ! %replace DTR$K_INF_FLD_CNT by 25; " %replace DTR$K_INF_FLD_LIST by 26;' %replace DTR$K_INF_FLD_REDEFINES by 27; % %replace DTR$K_INF_FLD_VIRTUAL by 28; $ %replace DTR$K_INF_FLD_FILLER by 29;% %replace DTR$K_INF_FLD_MISSING by 30; " %replace DTR$K_INF_FLD_QHDR by 31;" %replace DTR$K_INF_FRM_NAME by 32;% %replace DTR$K_INF_FRM_LIBRARY by 33; ' %replace DTR$K_INF_GLV_FIRST_DOM by 34; ' %replace DTR$K_INF_GLV_FIRST_COL by 35; ' %replace DTR$K_INF_GLV_FIRST_SSC by 36; % %replace DTR$K_INF_GLV_STA_OBJ by 37; % %replace DTR$K_INF_GLV_STA_CNT by 38; & %replace DTR$K_INF_GLV_STA_LINE by 39;! %replace DTR$K_INF_HDR_CNT by 40; $ %replace DTR$K_INF_HDR_STRING by 41;! %replace DTR$K_INF_PLO_CNT by 42; ! %replace DTR$K_INF_PLO_PAI by 43; $ %replace DTR$K_INF_PAI_PROMPT by 44;# %replace DTR$K_INF_PAI_DTYPE by 45; " %replace DTR$K_INF_SSC_NAME by 46;! %replace DTR$K_INF_SSC_SET by 47; " %replace DTR$K_INF_SSC_NEXT by 48;" %replace DTR$K_INF_SET_NAME by 49;" %replace DTR$K_INF_SET_NEXT by 50;! %replace DTR$K_INF_SET_SDP by 51; & %replace DTR$K_INF_SET_SINGULAR by 52;" %replace DTR$K_INF_SDP_NEXT by 53;$ %replace DTR$K_INF_SDP_DOMAIN by 54;% %replace DTR$K_INF_SDP_TENANCY by 55; $ %replace DTR$K_INF_SDP_INSERT by 56;$ %replace DTR$K_INF_SDP_RETAIN by 57;% %replace DTR$K_INF_DOM_REC_LEN by 58;   ! /* DTR$LOOKUP answer constants */ ( %replace DTR$K_INF_DOM_ACCESS_READ by 1;) %replace DTR$K_INF_DOM_ACCESS_WRITE by 2; * %replace DTR$K_INF_DOM_ACCESS_MODIFY by 3;* %replace DTR$K_INF_DOM_ACCESS_EXTEND by 4;  , %replace DTR$K_INF_DOM_SHARE_EXCLUSIVE by 1;) %replace DTR$K_INF_DOM_SHARE_SHARED by 2; * %replace DTR$K_INF_DOM_SHARE_PROTECT by 3;  # /* Variable declarations section */ 9 DCL 1 DTR_ACCESS_BLOCK based, /*Datatreive Access Block*/  	2 DAB$B_BID fixed binary(7),  	2 DAB$B_BLN fixed binary(7), $ 	2 DAB$L_CONDITION fixed binary(31), 	2 DAB$A_MSG_BUF pointer, & 	2 DAB$W_MSG_BUF_LEN fixed binary(15)," 	2 DAB$W_MSG_LEN fixed binary(15), 	2 DAB$A_AUX_BUF pointer, & 	2 DAB$W_AUX_BUF_LEN fixed binary(15)," 	2 DAB$W_AUX_LEN fixed binary(15), 	2 DAB$W_IDI fixed binary(15),  	2 DAB$W_STATE fixed binary(15), 	2 DAB$L_FLAGS bit(32) aligned, ! 	2 DAB$L_OPTIONS bit(32) aligned, % 	2 DAB$W_REC_LENGTH fixed binary(15), " 	2 DAB$W_VERSION fixed binary(15),  	2 DAB$W_LEVEL fixed binary(15), 	2 DAB$B_VER_LETTER character,% 	2 DAB$W_BASE_LEVEL fixed binary(15), $ 	2 DAB$W_UDK_INDEX fixed binary(15),' 	2 DAB$W_COLUMNS_PAGE fixed binary(15), % 	2 DAB$W_TT_CHANNEL fixed binary(15), ' 	2 DAB$W_CTLC_CHANNEL fixed binary(15), % 	2 DAB$L_KEYTABLE_ID bit(32) aligned, * 	2 DAB$L_COMMAND_KEYBOARD bit(32) aligned,) 	2 DAB$L_PROMPT_KEYBOARD bit(32) aligned,  	2 DAB$A_LOG_RAB pointer, & 	2 DAB$L_LMF_CONTEXT fixed binary(31),' 	2 DAB$L_RETURN_VALUE fixed binary(31), ( 	2 DAB$W_MAIN_WINDOW_X fixed binary(15),( 	2 DAB$W_MAIN_WINDOW_Y fixed binary(15),! 	2 DAB$REST_OF_DAB character(21);    DCL DTR$COMMAND entry($ 	1 like DTR_ACCESS_BLOCK,		/* dab */) 	any character(*),			/* command-string */  	any descriptor)	/* p1...pn */- 	returns(fixed binary(31)) options(variable);    DCL DTR$CONTINUE entry( $ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));    DCL DTR$CREATE_UDK entry( $ 	1�                                                                                                                                                                                                                                                   	                        ��� $      
DTRJA061.C                     ~  
  [C]DAB.PLI;1                                                                                                                   O   �                          �7 
             like DTR_ACCESS_BLOCK,		/* dab */' 	any character(*),			/* keyword-name */   	fixed binary(15),			/* index */" 	fixed binary(15))			/* context */ 	returns(fixed binary(31));    DCL DTR$DTR entry($ 	1 like DTR_ACCESS_BLOCK,		/* dab */& 	bit(32) aligned)			/* options-code */ 	returns(fixed binary(31));    DCL DTR$END_UDK entry($ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));    DCL DTR$FINISH entry( $ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));    DCL DTR$GET_PORT entry( $ 	1 like DTR_ACCESS_BLOCK,		/* dab */ 	any)					/* record-buffer */  	returns(fixed binary(31));    DCL DTR$GET_STRING entry( $ 	1 like DTR_ACCESS_BLOCK,		/* dab */% 	fixed binary(31),			/* token-type */ ! 	any character(*),			/* string */  	fixed binary(15),	/* length */ ' 	any character(*))	/* compare-string */ - 	returns(fixed binary(31)) options(variable);    DCL DTR$INFO entry( $ 	1 like DTR_ACCESS_BLOCK,		/* dab */$ 	fixed binary(31),			/* object-id */# 	fixed binary(7),			/* info-code */ " 	fixed binary(31),			/* ret-val */& 	any character(*),	/* output-string */$ 	fixed binary(31) value) /* index */- 	returns(fixed binary(31)) options(variable);    DCL DTR$INIT entry( $ 	1 like DTR_ACCESS_BLOCK,		/* dab */ 	fixed binary(31),	/* size */  	character(*),	/* msg-buff */  	character(*),	/* aux-buff */ $ 	bit(32) aligned)	/* options-code */- 	returns(fixed binary(31)) options(variable);    DCL DTR$LOOKUP entry( $ 	1 like DTR_ACCESS_BLOCK,		/* dab */% 	fixed binary(7),			/* object-type */ $ 	fixed binary(31),			/* object-id */$ 	any character(*))	/* object-name */- 	returns(fixed binary(31)) options(variable);    DCL DTR$PORT_EOF entry( $ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));x   DCL DTR$PRINT_DAB entry($ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));t   DCL DTR$PUT_OUTPUT entry(b$ 	1 like DTR_ACCESS_BLOCK,		/* dab */! 	any character(*),			/* string */ & 	any character(*))	/* prompt-string */- 	returns(fixed binary(31)) options(variable);    DCL DTR$PUT_PORT entry(r$ 	1 like DTR_ACCESS_BLOCK,		/* dab */ 	any)					/* record-buffer */t 	returns(fixed binary(31));I   DCL DTR$PUT_VALUE entry($ 	1 like DTR_ACCESS_BLOCK,		/* dab */ 	any character(*))	/* value */- 	returns(fixed binary(31)) options(variable);w   DCL DTR$UNWIND entry(E$ 	1 like DTR_ACCESS_BLOCK)		/* dab */ 	returns(fixed binary(31));R��0);   *L  *  If for some reason, you want to use your own declaration of the DAB, butK  *  still want to use the entry point definitions in thi�               * [C]DISPLAY.FOR;1 +  , �   .     / s P   4 O       �                    - 
    0  � 1    2   3      K � P   W   O     5   6 @e���"�  7 �3�)�*�  8          9          G s P  H �� J �              K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C  C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C 8 C*******************************************************. C                SUBROUTINE DISPLAY		       * 8 C Displays the current collection of records.          *8 C This program is a subroutine of the MENU.FOR program *8 C*******************************************************   	SUBROUTINE DISPLAY  	INCLUDE 'DTR$LIBRARY:DAB'0 	CHARACTER*27 TEXT/'PRESS RETURN TO CONTINUE >'/ 	CHARACTER*1  CR 	INTEGER*4    DTR_OPTIONS ! 	INTEGER*2    E_LINE/1/, E_COL/1/ # 	INTEGER*2    P_LINE/24/, P_COL/15/   # 	CALL LIB$ERASE_PAGE (E_LINE,E_COL)   & C Select terminal server call options.   	    DTR_OPTIONS =< 	1            DTR$M_OPT_CMD        ! Return on DTR$K_STL_CMD> 	2          + DTR$M_OPT_CONTROL_C  ! Enable Control C handling    6 C Instruct DATATRIEVE to print the current collection.  $ 	CALL DTR$COMMAND (DAB, 'PRINT ALL')  '         CALL DTR$DTR (DAB, DTR_OPTIONS)   F C Call a library routine to put a message at the bottom of the screen.   	WRITE (6, *)' ' 	WRITE (6, *)' '* 	CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL)  ) C Wait until the user has pressed RETURN.    	READ (5, 1) CR  1	FORMAT (A) 	RETURN    	END��                                                                                                                                                                                                                                                                                                                                      �                * [C]DTRFND_SINGLE_PRECISION.MAR;1 +  , �'   .     / s P   4 G      
 �                    - 
    0  � 1    2   3      K � P   W   O     5   6 @n�RK�  7  �)�*�  8          9          G s P  H �� J �               ;T4.2-1 1 ;Please do not remove the preceeding version line 6 .TITLE	DTRFND	VAX Datatrieve User Function Definitions4 ;+++++++++++++++++++++++++++++++++++++++++++++++++++ ; . ;     F U N C T I O N    D E F I N I T I O N S ; 4 ;---------------------------------------------------   .PSECT	FND,NOWRT,SHR,PIC,2  ) .LIBRARY /DL5:DTRFNLB/  ; for diagnostics    .LIBRARY /SYS$LIBRARY:STARLET/   ;.SHOW EXPANSIONS    $DSCDEF  $DTR$FUN_INIT    ; FN$ABS - Absolute value  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$ABS, MTH$ABS, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$ATAN - Arc tangent  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference     " $DTR$IFUN_DEF FN$ATAN, MTH$ATAN, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F ,     $DTR$FUN_HEADER  HDR = <"ARC"/"TANGENT">G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$COS - Cosine Function ; " ; output is a floating value in R0/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$COS, MTH$COS, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F "     $DTR$FUN_EDIT_STRING  ^\9.999\G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$EXP - Exponential Function  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$EXP, MTH$EXP, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF  & ; FN$FLOOR - Greatest floating integer ;  ; output is a floating number  ; input is a floating number    $ $DTR$IFUN_DEF FN$FLOOR, MTH$FLOOR, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$LN - Natural logarithm  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$LN, MTH$ALOG, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$LOG10 - Common logaritm ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference     % $DTR$IFUN_DEF FN$LOG10, MTH$ALOG10, 1 ?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$MOD - Modulo  ; 9 ; output is floating. Calculated by arg1-arg2*(arg1/arg2)  ; input is the floating operand  ; and the floating modulus    ! $DTR$IFUN_DEF FN$MOD, MTH$AMOD, 2 ?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1 G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 2  $DTR$FUN_END_DEF  ) ; FN$NINT - nearest integer from floating  ;  ; output is an integer ; input is a floating number    # $DTR$IFUN_DEF FN$NINT, MTH$JNINT, 1 ?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_L G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$SIGN - Sign of a number ; " ; output is a floating value in R0/ ; input is a floating �                                                                                                                                                                                                                                   
                        [�{� $      
DTRJA061.C                     �'  
   [C]DTRFND_SINGLE_PRECISION.MAR;1                                                                                               G                              i�             value passed by reference     ! $DTR$IFUN_DEF FN$SIGN, MTH$SGN, 1 ?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_L G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$SIN - Sine Function ; " ; output is a floating value in R0/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$SIN, MTH$SIN, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F "     $DTR$FUN_EDIT_STRING  ^\9.999\G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$SQRT - Square root  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference     " $DTR$IFUN_DEF FN$SQRT, MTH$SQRT, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   ; FN$TAN - Tangent function  ; & ; output is a floating value in R0, R1/ ; input is a floating value passed by reference       $DTR$IFUN_DEF FN$TAN, MTH$TAN, 1?     $DTR$FUN_OUT_ARG  TYPE = FUN$K_VALUE, DTYPE = DSC$K_DTYPE_F G     $DTR$FUN_IN_ARG  TYPE = FUN$K_REF, DTYPE = DSC$K_DTYPE_F, ORDER = 1  $DTR$FUN_END_DEF   $DTR$FUN_FINI    .END��                                                                                                                                                                                                                                                                                                    �               * [C]DTRMSGS.MSG;1 +  , �    . �    / s P   4     �   � �                    - 
    0  � 1    2   3      K � P   W   O �    5   6 ��i�*�  7 ���)�*�  8          9          G s P  H �� J �              .TITLE DTRMSGS VAX Datatrieve error messages
.FACILITY	DTR, 141 /SYSTEM /PREFIX=DTR$_
.IDENT 'VAX DTR T6.1-3'
.BASE 1
BADBLK/FAO=2/SEVERE-
<Internal error (expected block id !UB, encountered id !UB). >
.BASE 2
BADSUBBLK/FAO=2/SEVERE-
<Internal error (expected node type !UB, encountered type !UB). >
.BASE 3
BADVARTYP/FAO=0/SEVERE-
<Internal error (bad variable type in SYN$DEC block). >
.BASE 4
BLKTOOBIG/FAO=0/ERROR-
<Maximum block size exceeded; statement aborted. >
.BASE 5
INVBLKTYP/FAO=0/SEVERE-
<Internal error (invalid block type given). >
.BASE 6
INVDSCTYP/FAO=0/SEVERE-
<Invalid descriptor type for conversion. >
.BASE 7
INVPOOLCL/FAO=0/SEVERE-
<Internal error (invalid pool class given). >
.BASE 8
RELBADBLK/FAO=0/SEVERE-
<Internal error (attempted to release bad block). >
.BASE 9
RELBADPOL/FAO=0/SEVERE-
<Internal error (attempted to release bad pool). >
.BASE 10
NOTYETIMP/FAO=0/SEVERE-
<Internal error (feature is not implemented yet). >
.BASE 11
NOCURTOK/FAO=0/SEVERE-
<Internal error (no current token block). >
.BASE 12
SYSBADARG/FAO=0/SEVERE-
<Internal error (invalid number of arguments). >
.BASE 13
CASOUTRAN/FAO=0/SEVERE-
<Internal error (software consistency check). >
.BASE 14
NULROC/FAO=0/SEVERE-
<Internal error (null record occurrence block). >
.BASE 15
DOMLOST/FAO=0/SEVERE-
<Internal error (domain lost from system list). >
.BASE 16
HSHBLKLOS/FAO=0/SEVERE-
<Internal error (hash block lost). >
.BASE 17
HOPELESS/FAO=0/SEVERE-
<Internal error (software consistency check). >
.BASE 18
FSGNORID/FAO=0/SEVERE-
<Internal error (file segment missing record ID). >
.BASE 19
GETRIDFAIL/FAO=0/SEVERE-
<Internal error (failed to get record by RID). >
.BASE 20
NOKEEPNEE/FAO=0/SEVERE-
<Internal error (no keeplist when one is needed). >
.BASE 21
KPLNOSSC/FAO=0/SEVERE-
<Internal error (keeplist requested when no subschema is in use). >
.BASE 22
NULPRIELT/FAO=0/SEVERE-
<Internal error (null print element in context searcher). >
.BASE 23
ERROR/FAO=0/ERROR-
<Statement abandoned due to error. >
.BASE 24
NO_DFPID/FAO=0/ERROR-
<One of PW, UIC, USER, or TERMINAL must be specified. >
.BASE 25
NO_DFPPRI/FAO=0/ERROR-
<One of GRANT, DENY, or BANISH must be specified. >
.BASE 26
BADDFPTER/FAO=0/ERROR-
<Invalid TERMINAL specification for DEFINEP command. >
.BASE 27
BADUIC/FAO=0/ERROR-
<Invalid UIC specified. >
.BASE 28
PRIVVIOL/FAO=0/ERROR-
<Privilege violation -- access denied. >
.BASE 29
NOPOOLIDS/FAO=0/SEVERE-
<Internal error (all available dynamic memory pools have been exhausted). >
.BASE 30
BADHANDLE/FAO=0/ERROR-
<Invalid DDMF handle. >
.BASE 31
BADNUMARG/FAO=0/ERROR-
<Invalid number of arguments to DDMF. >
.BASE 32
USESLOEXH/FAO=0/ERROR-
<DDMF user slots are exhausted - cannot initialize another DATATRIEVE stream. >
.BASE 33
WRONGSTALL/FAO=0/ERROR-
<Wrong call for current stallpoint. >
.BASE 34
SYNTAX/FAO=3/ERROR-
<Expected !AC, encountered "!AD". >
.BASE 35
DUPCLASPE/FAO=2/ERROR-
<Duplicate clause specified: "!AD". >
.BASE 36
FLDNOTRED/FAO=2/ERROR-
<Field "!AD" not found for REDEFINES. >
.BASE 37
FLDNOTOCC/FAO=2/ERROR-
<Field "!AD" not found for OCCURS DEPENDING ON. >
.BASE 38
MULOCCDEP/FAO=0/ERROR-
<Multiple OCCURS DEPENDING ON clauses are not allowed. >
.BASE 39
OCCNOTLAS/FAO=0/ERROR-
<Only fields with higher level numbers are allowed after an OCCURS DEPENDING ON. >
.BASE 40
OCCOUTRAN/FAO=0/ERROR-
<OCCURS DEPENDING ON count out of range. >
.BASE 41
ILLPICSTR/FAO=2/ERROR-
<Illegal PICTURE or EDIT string character: "!AD". >
.BASE 42
PICUSAMAT/FAO=0/ERROR-
<PICTURE and USAGE clauses don't match in previous line. >
.BASE 43
TOOMANDIG/FAO=0/ERROR-
<Too many digits in numeric string (max is 18). >
.BASE 44
MISPICUSA/FAO=0/ERROR-
<Missing PICTURE or USAGE clause in previous line. >
.BASE 45
BADLENRED/FAO=2/ERROR-
<Bad length for REDEFINES, field "!AD". >
.BASE 46
LINTOOLON/FAO=0/ERROR-
<Command line exceeds 255 characters. >
.BASE 47
UNTERQUO/FAO=0/ERROR-
<Unterminated quoted string. >
.BASE 48
MISTERMIN/FAO=0/ERROR-
<Missing line terminator (semicolon). >
.BASE 49
DICELTUSE/FAO=1/ERROR-
<Dictionary element "!AS" already in use. >
.BASE 50
ELTNOTDIC/FAO=1/ERROR-
<Element "!AS" not found in dictionary. >
.BASE 51
ELTNOTDTR/FAO=1/ERROR-
<Element "!AS" is not a DATATRIEVE object. >
.BASE 52
DICNOTUND/FAO=0/ERROR-
<Dictionary element not understood. >
.BASE 53
NOTRDYDOM/FAO=2/ERROR-
<"!AD" is not a readied source. >
.BASE 54
DOMNOTRDY/FAO=1/ERROR-
<Domain "!AC" not ready. >
.BASE 55
NOTRECNAM/FAO=1/ERROR-
<"!AS" is not a record name. >
.BASE 56
NOTDOMNAM/FAO=1/ERROR-
<"!AS" is not a domain name. >
.BASE 57
NOTPRCNAM/FAO=1/ERROR-
<"!AS" is not a procedure name. >
.BASE 58
NOTTABNAM/FAO=1/ERROR-
<"!AS" is not a table name. >
.BASE 59
NOTPLTNAM/FAO=1/ERROR-
<"!AS" is not a plot name. >
.BASE 60
NOTDOMAIN/FAO=2/ERROR-
<"!AD" is not a readied source, collection, or list. >
.BASE 61
NOTLIST/FAO=2/SEVERE-
<"!AD" is not a list. >
.BASE 62
NOCONTEXT/FAO=2/ERROR-
<"!AD" is undefined or used out of context. >
.BASE 63
NULPRILIS/FAO=0/ERROR-
<Null print list. >
.BASE 64
BADNUMBER/FAO=2/ERROR-
<Bad number: "!AD". >
.BASE 65
MOV_RANGE/FAO=0/ERROR-
<Item movement beyond allowed range. >
.BASE 66
UNSCONV/FAO=0/ERROR-
<Unsupported data conversion. >
.BASE 67
UNWIND/FAO=0/ERROR-
<Execution terminated by operator. >
.BASE 68
COMPOVF/FAO=0/ERROR-
<Computation overflow. >
.BASE 69
UNXDICTYP/FAO=0/ERROR-
<Unexpected attribute type encountered in dictionary. >
.BASE 70
REQDICMIS/FAO=1/ERROR-
<Required dictionary attribute !AC missing. >
.BASE 71
WRIACCREQ/FAO=1/ERROR-
<Write access is required for "!AC". >
.BASE 72
MODACCREQ/FAO=1/ERROR-
<Modify access is required for  "!AC". >
.BASE 73
PATNOTDIR/FAO=1/ERROR-
<Path node "!AS" is not a directory. >
.BASE 74
DELDIRNOD/FAO=0/ERROR-
<Can't delete a directory node. >
.BASE 75
BADRECSIZ/FAO=2/WARNING-
<Bad record size. Defined: !UW  File: !UW >
.BASE 76
PRISTALOS/FAO=0/ERROR-
<Prior command lost.  Sorry. >
.BASE 77
TXTNOTFND/FAO=1/ERROR-
<Source text for !AS not found in dictionary. >
.BASE 78
BADROCKBF/FAO=0/SEVERE-
<Internal error (bad ROC chain or key buffer). >
.BASE 79
BADSORREC/FAO=0/SEVERE-
<Internal error (bad sort record length). >
.BASE 80
MISSUBPAR/FAO=0/ERROR-
<Missing substitution parameter in command string. >
.BASE 81
BADSUBPAR/FAO=0/ERROR-
<Bad substitution parameter in command string. >
.BASE 82
BADSUBDES/FAO=0/ERROR-
<Bad substitution descriptor argument. >
.BASE 83
EDTNOTINS/FAO=0/ERROR-
<EDT is not installed.  Try TECO. >
.BASE 84
RERDYFAIL/FAO=0/ERROR-
<Re-ready failed. Domain automatically finished. >
.BASE 85
NOCOMP6/FAO=0/ERROR-
<COMP-6 data type is not implemented. >
.BASE 86
RDSNOTACT/FAO=0/ERROR-
<Remote Serv�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �s�7 $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         �g "            er (DDMF) could not be activated. >
.BASE 87
RDSTERMIN/FAO=0/ERROR-
<Remote Server (DDMF) terminated unexpectedly. >
.BASE 88
DBMINCSSC/FAO=0/ERROR-
<Inconsistent DBMS subschema in dictionary. >
.BASE 89
DBMSSCDOM/FAO=0/ERROR-
<One subschema already in use - can't ready domain. >
.BASE 90
INFBADID/FAO=0/ERROR-
<Bad object id. >
.BASE 91
INFBADCOD/FAO=0/ERROR-
<Bad information code. >
.BASE 92
INFBADTYP/FAO=0/ERROR-
<Bad data type to receive information. >
.BASE 93
INFNOTFOU/FAO=0/ERROR-
<Object not found. >
.BASE 94
REMSYNERR/FAO=0/ERROR-
<DDMF remote synchronization error. >
.BASE 95
ABORT/FAO=1/ERROR-
<ABORT: !AS >
.BASE 96
NOTSET/FAO=2/ERROR-
<"!AD" is not a set. >
.BASE 97
MISSETCTX/FAO=2/ERROR-
<Set "!AD" is undefined or used out of context. >
.BASE 98
PAGEINUSE/FAO=0/SEVERE-
<Internal error (attempt to release a page in use). >
.BASE 99
VBNOUTRAN/FAO=0/SEVERE-
<Internal error (file's virtual block number out of range). >
.BASE 100
PAGOUTRAN/FAO=0/SEVERE-
<Internal error (page number out of range for file segment). >
.BASE 101
NOFREEBCB/FAO=0/SEVERE-
<Internal error (no free buffer control blocks). >
.BASE 102
NOTINMEM/FAO=0/SEVERE-
<Internal error (page is not in memory - can't release or mark). >
.BASE 103
PAGLSTUSR/FAO=0/SEVERE-
<Internal error (user of a page buffer got lost). >
.BASE 104
NOCURCOL/FAO=0/ERROR-
<A current collection has not been established. >
.BASE 105
NOCOLSEL/FAO=0/ERROR-
<No collection for select. >
.BASE 106
NOCOLSOR/FAO=0/ERROR-
<No collection for sort. >
.BASE 107
OUTRANCOL/FAO=0/ERROR-
<Record number out of range for collection. >
.BASE 108
SELNOTFND/FAO=0/ERROR-
<Selected record not found. >
.BASE 109
RECTOOBIG/FAO=0/ERROR-
<Record too big for sequential update. >
.BASE 110
WRODOMTYP/FAO=2/ERROR-
<"!AS" is not an RMS domain. >
.BASE 111
KEYUNDEF/FAO=4/ERROR-
<Key "!AF" is not defined in domain "!AF". >
.BASE 112
OBJTOOBIG/FAO=0/ERROR-
<Print object is too large for line width. >
.BASE 113
VALIDERR/FAO=2/ERROR-
<Validation error for field !AD. >
.BASE 114
ASSVIRFLD/FAO=0/ERROR-
<Cannot assign to a virtual field. >
.BASE 115
DIGNUMPIC/FAO=0/ERROR-
<Too many digits in numeric picture or edit string. >
.BASE 116
ZEROOBJ/FAO=0/INFORMATIONAL-
<Can't take MAX, MIN, or AVERAGE of zero objects. >
.BASE 117
NOTINTAB/FAO=0/WARNING-
<Value not found in table. >
.BASE 118
NOVALFROM/FAO=0/ERROR-
<Value not found from record or table. >
.BASE 119
NOTRGREC/FAO=1/ERROR-
<No target record for !AC. >
.BASE 120
CNVNUMDAT/FAO=0/ERROR-
<Can't convert number to a date string. >
.BASE 121
RECERSD/FAO=0/ERROR-
<You cannot use a field from a record that has been erased. >
.BASE 122
NODBMERAS/FAO=0/ERROR-
<Can't erase DBMS records - sorry. >
.BASE 123
SELRECREQ/FAO=0/ERROR-
<No selected record in collection. >
.BASE 124
RECNOTDBM/FAO=2/ERROR-
<Record in collection !AD is not DBMS record. >
.BASE 125
NOTMEMSET/FAO=3/ERROR-
<Domain !AC is not a member of set !AD. >
.BASE 126
NOTOWNSET/FAO=3/ERROR-
<Domain !AC is not an owner of set !AD. >
.BASE 127
IRRELESET/FAO=3/ERROR-
<Domain !AC is not related to set !AD. >
.BASE 128
NODBMDOM/FAO=0/INFORMATIONAL-
<No DBMS domains readied - nothing to COMMIT or ROLLBACK. >
.BASE 129
BADINSERT/FAO=2/ERROR-
<Insertion for set !AD precludes execution. >
.BASE 130
MISAUTSET/FAO=2/ERROR-
<AUTOMATIC set !AD must be specified for store. >
.BASE 131
RPOEXCPRI/FAO=0/ERROR-
<Excessive PRINT statements for REPORT. >
.BASE 132
RPOFMTRJT/FAO=0/ERROR-
<REPORT formatting clause out of context. >
.BASE 133
MISNOTFLD/FAO=0/ERROR-
<Object of MISSING operator must be a field. >
.BASE 134
PLONOTFOU/FAO=0/ERROR-
<Plot definition not found. >
.BASE 135
PLOENTMIS/FAO=0/ERROR-
<Plot entry point is not defined. >
.BASE 136
PLOBADARG/FAO=0/ERROR-
<Too many parameters for plot definition. >
.BASE 137
RPODUPAT/FAO=2/ERROR-
<Duplicate control break specified for !AD. >
.BASE 138
RPODUPSET/FAO=1/ERROR-
<Duplicate SET option !AC specified. >
.BASE 139
PLOIDXRAN/FAO=3/ERROR-
<Plot index out of range.  Vector: !AD, index: !SL >
.BASE 140
PLOLOCRAN/FAO=2/ERROR-
<Plot location fn out of range.  Fn: !AC, value: !SL >
.BASE 141
NOTVALPRI/FAO=2/ERROR-
<"!AD" is not a valid privilege code. >
.BASE 142
CLANOTPER/FAO=1/ERROR-
<!AC clause is not permitted on a variable declaration. >
.BASE 143
NOCOLDROP/FAO=0/ERROR-
<No collection with selected record for DROP. >
.BASE 144
RECPREDRO/FAO=0/ERROR-
<Record has been dropped from the collection. >
.BASE 145
VIEFIEUND/FAO=2/ERROR-
<Field !AS cannot be found in domain !AS. >
.BASE 146
VIEDOMUND/FAO=1/ERROR-
<Domain !AS was not included in domain list for view. >
.BASE 147
WARNING/FAO=0/WARNING-
<Statement completed with warning. >
.BASE 148
DATCNVERR/FAO=1/WARNING-
<Conversion error in date string "!AS". >
.BASE 149
COMPLEX/FAO=0/WARNING-
<Data conversion error on complex data type. >
.BASE 150
UNDERFLOW/FAO=0/WARNING-
<Data conversion underflow. >
.BASE 151
NONDIGIT/FAO=1/WARNING-
<Non-digit in string "!AS", ignoring character(s). >
.BASE 152
CONSIGUSA/FAO=1/WARNING-
<Conflicting SIGN and USAGE clauses, ignoring "!AC". >
.BASE 153
CONVALCOM/FAO=0/WARNING-
<Conflicting VALID and COMPUTED clauses. >
.BASE 154
OVERFLOW/FAO=0/WARNING-
<Data conversion overflow. >
.BASE 155
NOACCCHG/FAO=0/WARNING-
<Couldn't change access to readied domain. >
.BASE 156
ASSUMELIT/FAO=2/WARNING-
<"!AD" not field, assumed literal. >
.BASE 157
DICELECOR/FAO=2/WARNING-
<Dictionary element "!AD" is corrupt and cannot be accessed. >
.BASE 158
DIRCMPSTK/FAO=1/SEVERE-
<Internal error (dirty compiler stack, level !UL). >
.BASE 159
DIVBYZERO/FAO=0/WARNING-
<Attempt to divide by zero. Returning -1 as a value. >
.BASE 160
TRUDURASS/FAO=0/WARNING-
<Truncation during assignment. >
.BASE 161
OVFDURMUL/FAO=0/WARNING-
<Overflow during multiplication. >
.BASE 162
COMPURCOL/FAO=2/WARNING-
<COMMIT or ROLLBACK executed; collection !AD automatically released. >
.BASE 163
STAMISDAT/FAO=2/INFORMATIONAL-
<[Function computed using !UL of !UL values.] >
.BASE 164
MISCLAMIS/FAO=2/WARNING-
<MISSING VALUE not defined for !AD, using default value. >
.BASE 165
NORECSEL/FAO=0/WARNING-
<No record selected, printing whole collection. >
.BASE 166
EXPCOTAVA/FAO=1/WARNING-
<Expected collection, table, or variable, encountered "!AS". >
.BASE 167
CTXMAGIC/FAO=0/WARNING-
<Not enough context. Some field names resolved by Context Searcher. >
.BASE 168
PROMPT/FAO=1/INFORMATIONAL-
<Enter !AS: >
.BASE 169
RE_PROMPT/FAO=1/INFORMATIONAL-
<Re-enter !AS: >
.BASE 170
PW_PROMPT/FAO=1/INFORMATIONAL-
<Enter password for !AS: >
.BASE 171
SIGNON/FAO=4/INFORMATIONAL-
<DEC DATATRIEVE !AD!UL.!UL!/Digital Query and Report System!/Type HELP for help!/ >
.BASE 172
SUCCESS/FAO=0/INFORMATIONAL-
<Statement completed successfully. >
.BASE 173
SHUTDOWN/FAO=0/INFORMATIONAL-
<Shut down DATATRIEVE/DDMF. >
.BASE 175
LOOKINFOR/FAO=1/INFORMATIONAL-
<[Looking for !AC] >
.BASE 176
FLUSH/FAO=0/INFORMATIONAL-
<Syntax error, flushing input. >
.BASE 177
RECLENGTH/FAO=1/INFORMATIONAL-
<[Record is !UL byte!%S long.] >
.BASE 178
DICTIONARY/FAO=1/INFORMATIONAL-
<The default directory is !AC >
.BASE 179
DOMAINS/FAO=0/INFORMATIONAL-
<Domains: >
.BASE 180
NODOMGLO/FAO=0/INFORMATIONAL-
<No ready sources or global variables declared. >
.BASE 181
PROCEDURES/FAO=0/INFORMATIONAL-
<Procedures: >
.BASE 182
PLOTS/FAO=0/INFORMATIONAL-
<Plots: >
.BASE 183
NOREADOM/FAO=0/INFORMATIONAL-
<No ready sources. >
.BASE 184
READYDOM/FAO=0/INFORMATIONAL-
<Ready domains: >
.BASE 185
RECORDS/FAO=0/INFORMATIONAL-
<Records: >
.BASE 186
TABLES/FAO=0/INFORMATIONAL-
<Tables: >
.BASE 187
DICTIONARIES/FAO=0/INFORMATIONAL-
<Dictionaries: >
.BASE 188
DELNOTFOU/FAO=0/INFORMATIONAL-
<Dictionary object does not exist. >
.BASE 189
EDTINPUT/FAO=0/INFORMATIONAL-
<EDT wants input. >
.BASE 190
EDTOUTPUT/FAO=1/INFORMATIONAL-
<EDT has output: !AS. >
.BASE 191
EDTEXIT/FAO=0/INFORMATIONAL-
<EDT is exiting. >
.BASE 192
REMOTEMSG/FAO=2/INFORMATIONAL-
<[DDMF] !AD. >
.BASE 193
DISPLAY/FAO=1/INFORMATIONAL-
<DISPLAY: !AS >
.BASE 194
RECFOUND/FAO=1/INFORMATIONAL-
<[!UL record!%S found] >
.BASE 195
REMPACKET/FAO=1/INFORMATIONAL-
<Packet received, type !UL. >
.BASE 196
COLLECTIONS/FAO=0/INFORMATIONAL-
<Collections: >
.BASE 197
NOESTCOLL/FAO=0/INFORMATIONAL-
<No established collections. >
.BASE 198
NOSETSUSE/FAO=0/INFORMATIONAL-
<No sets are currently useful. >
.BASE 199
BACKTRACE/FAO=2/INFORMATIONAL-
<!AD >
.BASE 200
VIRTUALFLD/FAO=0/INFORMATIONAL-
<Virtual field. >
.BASE 201
SHOCOLCOL/FAO=1/INFORMATIONAL-
<Collection !AC >
.BASE 202
SHOCOLDOM/FAO=1/INFORMATIONAL-
<    Domain: !AC >
.BASE 203
SHOCOLNREC/FAO=1/INFORMATIONAL-
<    Number of Records: !SL >
.BASE 204
SHOCOLSRN/FAO=1/INFORMATIONAL-
<    Selected Record: !SL >
.BASE 205
SHOCOLSRNE/FAO=1/INFO�                                                                                                                                                                                                                                                                           ���. $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         k "            RMATIONAL-
<    Selected Record: !SL (Erased) >
.BASE 206
SHOCOLSRND/FAO=1/INFORMATIONAL-
<    Selected Record: !SL (Dropped) >
.BASE 207
SHOCOLNSR/FAO=0/INFORMATIONAL-
<    No Selected Record >
.BASE 209
VARIABLES/FAO=0/INFORMATIONAL-
<Global variables >
.BASE 210
BADMISLEN/FAO=0/WARNING-
<Missing value edit string longer than edit string. >
.BASE 211
REPNOTFIT/FAO=0/ERROR-
<Print object(s) too long to fit on report line. >
.BASE 213
MAXLINEXC/FAO=0/ERROR-
<Maximum line count exceeded - report terminated. >
.BASE 214
NEWPINBOT/FAO=0/ERROR-
<NEW-PAGE and NEW-SECTION are illegal in AT TOP or BOTTOM OF PAGE print list. >
.BASE 215
BADQHDR/FAO=0/ERROR-
<Illegal report name. >
.BASE 216
SHOCOLSOR/FAO=2/INFORMATIONAL-
<    Sort order: !AD >
.BASE 217
SHOCOLSR2/FAO=2/INFORMATIONAL-
<                !AD >
.BASE 219
MAXPAGEXC/FAO=0/ERROR-
<Maximum report pages exceeded - report terminated. >
.BASE 220
NOGROPRMT/FAO=0/ERROR-
<Illegal assignment to a group data item. >
.BASE 221
INVCOLPAG/FAO=0/ERROR-
<Invalid number of columns for Set Columns-Page. >
.BASE 222
GUIINVTER/FAO=0/ERROR-
<Invalid terminal type for Guide Mode. >
.BASE 223
GPRMORE/FAO=0/INFORMATIONAL-
<More? (type N for no): >
.BASE 224
GPRCOMAND/FAO=0/INFORMATIONAL-
<enter command, type ? for help >
.BASE 225
GPRDOMAIN/FAO=0/INFORMATIONAL-
<domain name >
.BASE 226
GPRRDYMOD/FAO=0/INFORMATIONAL-
"ready mode or <return> "
.BASE 227
GPRWITH/FAO=0/INFORMATIONAL-
"WITH or <return> "
.BASE 228
GPRFIELD/FAO=0/INFORMATIONAL-
<field name >
.BASE 229
GPRSHOCAT/FAO=0/INFORMATIONAL-
<category >
.BASE 230
GPRCOLECT/FAO=0/INFORMATIONAL-
<collection name >
.BASE 231
GUILEAVE/FAO=0/INFORMATIONAL-
<You're on your own now.  Good luck! >
.BASE 232
ADTBADKEY/FAO=0/SEVERE-
<Invalid ADT HELP File Structure. >
.BASE 233
ADTAKBOVF/FAO=0/ERROR-
<Maximum key count for ADT exceeded. >
.BASE 234
ADTANBOVF/FAO=0/ERROR-
<Maximum field count for ADT exceeded. >
.BASE 235
ADTEXIT/FAO=0/ERROR-
<ADT exited by user request. >
.BASE 236
TARRECDRO/FAO=0/INFORMATIONAL-
<Target record has already been dropped. >
.BASE 237
GHEMODIFY/FAO=0/INFORMATIONAL-
<Update field values for the selected record >
.BASE 238
GHEPRINT/FAO=0/INFORMATIONAL-
<Print a record or records >
.BASE 239
GHESORT/FAO=0/INFORMATIONAL-
<Re-arrange the current collection >
.BASE 240
GHESELECT/FAO=0/INFORMATIONAL-
<Pick out a particular record >
.BASE 241
GHEFIND/FAO=0/INFORMATIONAL-
<Retrieve a collection of records >
.BASE 242
GHESTORE/FAO=0/INFORMATIONAL-
<Add a new record to a domain >
.BASE 243
GHEREADY/FAO=0/INFORMATIONAL-
<Make a domain available >
.BASE 244
GHESHOW/FAO=0/INFORMATIONAL-
<Display status information >
.BASE 245
GHELEAVE/FAO=0/INFORMATIONAL-
<Return to normal DATATRIEVE >
.BASE 246
GUIPOSRES/FAO=0/INFORMATIONAL-
<Possible responses are: >
.BASE 247
GUIBRETB/FAO=0/INFORMATIONAL-
"<return> "
.BASE 248
GUIBADCHR/FAO=0/INFORMATIONAL-
<Unusable character, type ? to get valid responses. >
.BASE 249
GPRDOMCOL/FAO=0/INFORMATIONAL-
<domain or collection name >
.BASE 250
GHEDOMAIN/FAO=0/INFORMATIONAL-
<Record domain >
.BASE 251
GHECOLECT/FAO=0/INFORMATIONAL-
<Record collection >
.BASE 252
NOFMS/FAO=0/SEVERE-
<DATATRIEVE has not been installed with forms support. >
.BASE 253
GHEWITH/FAO=0/INFORMATIONAL-
<Qualify record search >
.BASE 254
GPRCOMPAR/FAO=0/INFORMATIONAL-
<comparison >
.BASE 255
GHEALPHA/FAO=0/INFORMATIONAL-
<Alpha/numeric field >
.BASE 256
GHEALPHAQ/FAO=2/INFORMATIONAL-
<Alpha/numeric field (alias for !AC) >
.BASE 257
GHENUMBR/FAO=0/INFORMATIONAL-
<Numeric field >
.BASE 258
GHENUMBRQ/FAO=2/INFORMATIONAL-
<Numeric field (alias for !AC) >
.BASE 259
GHEDATE/FAO=0/INFORMATIONAL-
<Date field >
.BASE 260
GHEDATEQ/FAO=2/INFORMATIONAL-
<Date field (alias for !AC) >
.BASE 261
GHECOMPU/FAO=0/INFORMATIONAL-
<A computed value >
.BASE 262
GHECOMPUQ/FAO=2/INFORMATIONAL-
<A computed value (alias for !AC) >
.BASE 263
GHELIST/FAO=0/INFORMATIONAL-
<List >
.BASE 264
GHELISTQ/FAO=2/INFORMATIONAL-
<List (alias for !AC) >
.BASE 265
GHEGROUP/FAO=0/INFORMATIONAL-
<Group field >
.BASE 266
GHEGROUPQ/FAO=2/INFORMATIONAL-
<Group field (alias for !AC) >
.BASE 267
GUIEXECUT/FAO=0/INFORMATIONAL-
<[Executing...] >
.BASE 268
GUILEAVNG/FAO=0/INFORMATIONAL-
<You're on your own now.  Good luck! >
.BASE 269
GPRQUOTE/FAO=0/INFORMATIONAL-
<character string, end with a quote >
.BASE 270
GPRCOMRET/FAO=0/INFORMATIONAL-
"comma or <return> "
.BASE 271
GPRFLDRET/FAO=0/INFORMATIONAL-
"fieldname list or <return> "
.BASE 272
GPRALLFLD/FAO=0/INFORMATIONAL-
"ALL, fieldname list or <return> "
.BASE 273
CHAOPTINV/FAO=0/ERROR-
<The "CHANGE" option is invalid on the primary key of a file. >
.BASE 274
LOADEDTAB/FAO=1/INFORMATIONAL-
<!/Loaded tables: >
.BASE 275
NOLOADTAB/FAO=0/INFORMATIONAL-
<No loaded tables. >
.BASE 276
GHEALL/FAO=0/INFORMATIONAL-
<Use all of the records in the current collection >
.BASE 277
GPRFLDDEC/FAO=0/INFORMATIONAL-
<fieldname or DECREASING >
.BASE 278
GHEDECREA/FAO=0/INFORMATIONAL-
<Sort backwards >
.BASE 279
SHOSETUP/FAO=0/INFORMATIONAL-
<Set-up: >
.BASE 280
SHOCOLPAG/FAO=1/INFORMATIONAL-
<    Columns-page: !UL >
.BASE 281
SHONOABO/FAO=0/INFORMATIONAL-
<    No abort >
.BASE 282
SHOABORT/FAO=0/INFORMATIONAL-
<    Abort >
.BASE 283
SHONOFOR/FAO=0/INFORMATIONAL-
<    No form >
.BASE 284
SHOFORM/FAO=0/INFORMATIONAL-
<    Form >
.BASE 285
SHONOSEA/FAO=0/INFORMATIONAL-
<    No search >
.BASE 286
SHOSEARCH/FAO=0/INFORMATIONAL-
<    Search >
.BASE 287
SHONOPRO/FAO=0/INFORMATIONAL-
<    No prompt >
.BASE 288
SHOPROMPT/FAO=0/INFORMATIONAL-
<    Prompt >
.BASE 289
GHERETURN/FAO=0/INFORMATIONAL-
<Finish statement >
.BASE 290
GHENEXT/FAO=0/INFORMATIONAL-
<Next record of the collection (default case) >
.BASE 291
GHEPRIOR/FAO=0/INFORMATIONAL-
<Previous record of the collection >
.BASE 292
GPRSELOPT/FAO=0/INFORMATIONAL-
"which record or <return> "
.BASE 293
GHEGE/FAO=0/INFORMATIONAL-
<Greater than or equal to >
.BASE 294
GHEGT/FAO=0/INFORMATIONAL-
<Greater than >
.BASE 295
GHELE/FAO=0/INFORMATIONAL-
<Less than or equal to >
.BASE 296
GHELT/FAO=0/INFORMATIONAL-
<Less than >
.BASE 297
GHECONTAI/FAO=0/INFORMATIONAL-
<Containing a substring >
.BASE 298
GHEREADYR/FAO=0/INFORMATIONAL-
<Read access only >
.BASE 299
GHEREADYM/FAO=0/INFORMATIONAL-
<Read and modify access only >
.BASE 300
GHEREADYW/FAO=0/INFORMATIONAL-
<Read, modify and store access >
.BASE 301
EXPINVPIC/FAO=0/ERROR-
<Exponential edit-string is invalid for PICTURE clause. >
.BASE 302
CONTPREQ/FAO=0/ERROR-
<All "P"s must be contiguous in PICTURE or EDIT string. >
.BASE 303
ONEEPERM/FAO=0/ERROR-
<Only one "E" permitted in PICTURE or EDIT string. >
.BASE 304
NOTDBNAM/FAO=1/ERROR-
<"!AS" is not a database name. >
.BASE 305
DATABASES/FAO=0/INFORMATIONAL-
<Databases: >
.BASE 306
GHEANY/FAO=0/INFORMATIONAL-
<Search list >
.BASE 307
GPRLIST/FAO=0/INFORMATIONAL-
<list >
.BASE 308
NOCTXCON/FAO=2/ERROR-
<No context found for "!AD". >
.BASE 309
NODBMS/FAO=0/ERROR-
<The DBMS interface is not included in this image. >
.BASE 310
GUINXTCMD/FAO=0/INFORMATIONAL-
<Ready for next command >
.BASE 311
BADSTRDES/FAO=0/ERROR-
<Invalid string descriptor. >
.BASE 312
NOGLOVAR/FAO=0/INFORMATIONAL-
<No global variables are declared. >
.BASE 313
DUPUSACLA/FAO=2/ERROR-
<Duplicate USAGE clause specified: "!AD". >
.BASE 314
ILLASCNUM/FAO=1/WARNING-
<Illegal ASCII numeric "!AS". >
.BASE 315
SHOPRIVR/FAO=0/INFORMATIONAL-
< R (DTR_READ)		- may ready for READ, use SHOW and EXTRACT >
.BASE 316
SHOPRIVW/FAO=0/INFORMATIONAL-
< W (DTR_WRITE)		- may ready for READ, WRITE, MODIFY, or EXTEND >
.BASE 317
SHOPRIVM/FAO=0/INFORMATIONAL-
< M (DTR_MODIFY)		- may ready for READ, MODIFY >
.BASE 318
SHOPRIVE/FAO=0/INFORMATIONAL-
< E (DTR_EXTEND/EXECUTE) - may ready to EXTEND, or access table or procedure >
.BASE 319
SHOPRIVC/FAO=0/INFORMATIONAL-
< C (CONTROL)		- may issue DEFINEP, SHOWP, DELETEP commands >
.BASE 320
SHOPRIVD/FAO=0/INFORMATIONAL-
< D (LOCAL_DELETE)	- may delete a dictionary object >
.BASE 321
SHOPRIVF/FAO=0/INFORMATIONAL-
< F (FORWARD)		- may create a subdictionary >
.BASE 322
SHOPRIVG/FAO=0/INFORMATIONAL-
< G (GLOBAL_DELETE)	- may delete a directory and its descendents >
.BASE 323
SHOPRIVH/FAO=0/INFORMATIONAL-
< H (HISTORY)		- may add entries to object's history list >
.BASE 324
SHOPRIVP/FAO=0/INFORMATIONAL-
< P (PASS_THRU)		- may use given name of directory or object in pathname >
.BASE 325
SHOPRIVS/FAO=0/INFORMATIONAL-
< S (SEE)		- may see (read) dictionary >
.BASE 326
SHOPRIVU/FAO=0/INFORMATIONAL-
< U (UPDATE)		- may update dictionary object >
.BASE 327
SHOPRIVX/FAO=0/INFORMATIONAL-
< X (EXTEND)		- may create directory or object within directory >
.BASE 328
SHOPRIVNO/FAO=0/INFORMATIONAL-
< No privileges to directory or dictionary object. >
.BASE 329
F�                                                                                                                                                                                                                                                                           �R $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         �� "     0       ILNOTFND/FAO=1/ERROR-
<File not found - "!AC". >
.BASE 330
SELPURCOL/FAO=2/INFORMATIONAL-
<Parent collection de-selected, collection !AD automatically released. >
.BASE 331
PROVERMIS/FAO=2/SEVERE-
<Distributed protocol version mismatch. Host = !UL Server = !UL >
.BASE 332
ROLLBACK/FAO=0/WARNING-
<COMMIT failed, automatic ROLLBACK in progress. >
.BASE 333
NOREMCROS/FAO=0/ERROR-
<A remote domain may not participate in a CROSS. >
.BASE 334
ILLNEGNUM/FAO=0/WARNING-
<Meaningless negative value; zero used instead. >
.BASE 335
TOOMANSIG/FAO=1/WARNING-
<Too many signs in string "!AS", ignoring all but first. >
.BASE 336
TOOMANDEC/FAO=1/WARNING-
<Too many decimal points in string "!AS", ignoring all but first. >
.BASE 337
DATINPERR/FAO=1/WARNING-
<Error translating DTR$DATE_INPUT (translated value "!AS"). >
.BASE 338
NOVIEWSTO/FAO=1/ERROR-
<Illegal STORE into a view. >
.BASE 340
CANRDYDOM/FAO=1/ERROR-
<Source cannot be readied at this access mode !/unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 341
CANRRDYDM/FAO=1/WARNING-
<Source cannot be re-readied at this access mode !/unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 342
BADFLDCTX/FAO=2/ERROR-
<Invalid context for field "!AD". >
.BASE 343
OCCREQFRO/FAO=0/ERROR-
<OCCURS clause requires at least one FROM field at a lower level. >
.BASE 344
SHOWTEXT/FAO=2/INFORMATIONAL-
<!AD >
.BASE 345
NOSTOREL/FAO=1/ERROR-
<Can't STORE into a relative file (domain !AC). >
.BASE 346
NOERASEQ/FAO=1/ERROR-
<Can't ERASE from a sequential file (domain !AC). >
.BASE 347
NOREMANY/FAO=0/ERROR-
<Remote ANY or IN expressions are not supported. >
.BASE 348
SHOWPROC/FAO=2/INFORMATIONAL-
<PROCEDURE !AD >
.BASE 349
NOERALIS/FAO=0/ERROR-
<Can't ERASE from an OCCURS list. >
.BASE 350
NOERACRO/FAO=0/ERROR-
<Can't ERASE from a CROSS. >
.BASE 351
NOREMEXP/FAO=0/ERROR-
<Remote expression is not supported. >
.BASE 352
ASSNEGUNS/FAO=0/WARNING-
<Assignment of negative value to unsigned item; absolute value used. >
.BASE 353
MAGICFAIL/FAO=0/INFORMATIONAL-
<Context Searcher failed trying to resolve field. >
.BASE 354
BADDATEOP/FAO=0/ERROR-
<Cannot multiply or divide dates. >
.BASE 355
GUINOMORE/FAO=0/INFORMATIONAL-
<No more >
.BASE 356
SELBADBOO/FAO=0/ERROR-
<Unable to select a record that satisfies the Boolean. >
.BASE 357
SIGNON_FT/FAO=5/INFORMATIONAL-
<DEC DATATRIEVE !AD!UL.!UL-!UL!/Digital Query and Report System!/Type HELP for help!/ >
.BASE 358
NAMTOOLONG/FAO=0/ERROR-
<Field name can't be longer than 31 characters. >
.BASE 359
GPRCONT/FAO=0/INFORMATIONAL-
<Type any character to continue: >
.BASE 360
NOEXTRACT/FAO=0/ERROR-
<That record contains things that can't be expressed in the DATATRIEVE language. >
.BASE 361
SORTOPEN/FAO=0/ERROR-
<Error creating the work files for the sort. >
.BASE 362
HELPINIT/FAO=0/ERROR-
<An error occured while trying to initialize the HELP library. >
.BASE 363
HELPOPEN/FAO=0/ERROR-
<An error occured while trying to open the HELP library. >
.BASE 364
SETNOTUSE/FAO=2/ERROR-
<Set "!AD" cannot be used - either an owner or a member source is not ready. >
.BASE 365
BADPORTOP/FAO=0/ERROR-
<It is invalid to MODIFY, ERASE, FIND, SORT or REDUCE a port. >
.BASE 366
TOOMNYFLD/FAO=0/ERROR-
<Maximum number of fields exceeded. >
.BASE 367
GIVNAMUSE/FAO=3/ERROR-
<Given name "!AD" is already used by source !AC. >
.BASE 368
FLDSELFREF/FAO=2/ERROR-
<Field "!AD" contains a self-reference and cannot be evaluated. >
.BASE 369
OCCDEPOCC/FAO=0/ERROR-
<An OCCURS DEPENDING ON is not allowed within an OCCURS list. >
.BASE 371
PURETEST/FAO=0/INFORMATIONAL-
<This is purely a test >
.BASE 373
MINE/FAO=2/INFORMATIONAL-
<Internal coding error. >
.BASE 374
NOPROJFLD/FAO=0/ERROR-
<At least one reduce key must be a field defined in the record. >
.BASE 375
NOCOLPRO/FAO=0/ERROR-
<No collection for REDUCE statement. >
.BASE 376
OPENFILE/FAO=2/INFORMATIONAL-
<Creating file !AD ... >
.BASE 377
OPENTERM/FAO=2/INFORMATIONAL-
<Sending output to terminal !AD. >
.BASE 378
OPENLP/FAO=2/INFORMATIONAL-
<Spooling output to !AD. >
.BASE 379
ELSEREQ/FAO=0/ERROR-
<An ELSE clause is required for IF and CHOICE values. >
.BASE 380
ENDOFSTR/FAO=0/WARNING-
<End of string - no more tokens available for user defined keyword. >
.BASE 381
MORESTR/FAO=0/INFORMATIONAL-
<There are more tokens available for user defined keyword. >
.BASE 382
TRUNCSTR/FAO=0/WARNING-
<Truncated a token.  Remainder returned on next call. >
.BASE 383
VIDEOHELP/FAO=0/INFORMATIONAL-
" H E L P  -  type <PF2> for help.  Type <RETURN> to change help topics. "
.BASE 384
SHOWHELP/FAO=4/INFORMATIONAL-
<Help Settings: >
.BASE 385
HELPLINES/FAO=2/INFORMATIONAL-
<    Lines !UL to !UL >
.BASE 386
BADHLPLIN/FAO=2/WARNING-
<Help line number out of range or not enough lines. Top: !SL  Bottom: !SL. >
.BASE 387
NOTINFDI/FAO=0/ERROR-
<GET_FORM and PUT_FORM are allowed only in DISPLAY_FORM statements. >
.BASE 388
NESTFDI/FAO=0/ERROR-
<You cannot nest DISPLAY_FORM statements within DISPLAY_FORM. >
.BASE 389
INVNODSPC/FAO=0/ERROR-
<Invalid Node Specification. >
.BASE 390
SYNALDEC/FAO=2/WARNING-
<"!AD" is a keyword or is already declared as a synonym. >
.BASE 391
NODISCO/FAO=0/ERROR-
<CONNECT, DISCONNECT, and RECONNECT don't work on remote domains. >
.BASE 392
GPRPLOT/FAO=0/INFORMATIONAL-
<plot name >
.BASE 393
GHEEDIT/FAO=0/INFORMATIONAL-
<Enter the DATATRIEVE editor >
.BASE 394
GPRPROC/FAO=0/INFORMATIONAL-
<procedure name >
.BASE 395
GHEPROC/FAO=0/INFORMATIONAL-
<Invoke a DATATRIEVE procedure >
.BASE 396
GHEPLOT/FAO=0/INFORMATIONAL-
<Graph data from the current collection >
.BASE 397
GHECROSS/FAO=0/INFORMATIONAL-
<Join another record source to prior sources >
.BASE 398
GHEOVER/FAO=0/INFORMATIONAL-
<Associate record sources using a common field >
.BASE 399
GPRCROSS/FAO=0/INFORMATIONAL-
"CROSS, WITH or <return> "
.BASE 400
GPROVER/FAO=0/INFORMATIONAL-
"CROSS, OVER, WITH or <return> "
.BASE 401
BADUDKCTX/FAO=0/ERROR-
<Invalid parsing context specified for User Defined Keyword. >
.BASE 402
BADUDKIDX/FAO=0/ERROR-
<Invalid index specified for User Defined Keyword. >
.BASE 403
UDKUNWIND/FAO=0/INFORMATIONAL-
<Unwind UDK processing one level. >
.BASE 404
BADTOKTYP/FAO=0/ERROR-
<Invalid token type requested. >
.BASE 405
GPRNUMBER/FAO=0/INFORMATIONAL-
<number >
.BASE 406
BADSTALL/FAO=0/SEVERE-
<Stall point in DAB is invalid. >
.BASE 407
EXIT/FAO=0/SUCCESS-
<Exit from DATATRIEVE requested by operator. >
.BASE 408
INPLINTRU/FAO=0/ERROR-
<Input line too long - truncated to 255 characters. >
.BASE 409
NOPGETPUT/FAO=0/ERROR-
<Storing and retrieving from ports is not allowed in interactive DATATRIEVE. >
.BASE 410
NOUDKS/FAO=0/ERROR-
<User Defined Keywords are not allowed in interactive DATATRIEVE. >
.BASE 411
NOCOMFILE/FAO=0/ERROR-
<Expected indirect command file name, encountered end of line. >
.BASE 412
GPROPER/FAO=0/INFORMATIONAL-
<Optional arithmetic operator >
.BASE 413
NOLOGFILE/FAO=0/ERROR-
<Expected log file name, encountered end of line. >
.BASE 414
REMCMD/FAO=2/INFORMATIONAL-
"REMCMD> !AD "
.BASE 415
SHONOVERI/FAO=0/INFORMATIONAL-
<    No verify >
.BASE 416
SHOVERIFY/FAO=0/INFORMATIONAL-
<    Verify >
.BASE 417
HLPNOPMPT/FAO=0/INFORMATIONAL-
<    No prompting >
.BASE 418
HLPPROMPT/FAO=0/INFORMATIONAL-
<    Prompting >
.BASE 419
HLPNOWNDW/FAO=0/INFORMATIONAL-
<    No window >
.BASE 420
HLPWINDOW/FAO=0/INFORMATIONAL-
<    Window >
.BASE 421
ADTENOTNULL/FAO=0/WARNING-
<Do not use all spaces and/or tabs. >
.BASE 422
ADTECANTOPEN/FAO=1/WARNING-
<ADT could not open a file with that name.!/Please enter another file name. >
.BASE 423
ADTEKEYWORD/FAO=1/WARNING-
<That name duplicates a DATATRIEVE keyword.!/Please enter another name. >
.BASE 424
ADTEDUPNAME/FAO=1/WARNING-
<!AC duplicates another name.!/Please enter a unique name. >
.BASE 425
ADTEINVTYPE/FAO=1/WARNING-
<Please answer with DATE, PERCENT, MONEY, NUMBERS,!/CHARACTERS, or GROUP.!/You can also use the abbreviations D, P, M, N, C, or G. >
.BASE 426
ADTEINVFILE/FAO=1/WARNING-
<Please use a valid file name. Enter up to 9 letters or digits,!/and  optionally a period and up to 3 letters or digits. >
.BASE 427
ADTEINVDATE/FAO=0/WARNING-
<Please use one of these format numbers: 1, 2, 3, or 4. >
.BASE 428
ADTEKEYDOM/FAO=1/WARNING-
<Do not use the domain name as the name of the key field.!/Please enter the name of a field. >
.BASE 429
ADTENODOM/FAO=0/WARNING-
<You have not defined any domains or fields. >
.BASE 430
ADTENOTNUM/FAO=0/WARNING-
<Please answer with an integer. >
.BASE 431
ADTENOTPOS/FAO=0/WARNING-
<Please answer with a number greater than zero. >
.BASE 432
ADTENOTCR/FAO=0/WARNING-
<Do not respond by only pressing RETURN. >
.BASE 433
ADTENOTFLD/FAO=1/WARNING-
<To specify an index key name, use the name of a field!/you have previously defined. >
.BASE�                                                                                                                                                                                                                                                                           �H� $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         4 "     A        434
ADTENOTUNQ/FAO=0/WARNING-
<You have already used this field as a key. >
.BASE 435
ADTENUMBIG/FAO=1/WARNING-
<This field is too long for the type of data it can contain.!/Its size cannot exceed 18 digits. >
.BASE 436
ADTENUMSML/FAO=1/WARNING-
<The total number of digits is zero.!/Please enter a larger number of digits. >
.BASE 437
ADTENAMBIG/FAO=1/WARNING-
<This name exceeds the 30-character limit for DATATRIEVE names.!/Please enter another name. >
.BASE 438
ADTEFLDSML/FAO=1/WARNING-
<A field in a record cannot have a length of zero.!/Please answer with an integer. >
.BASE 439
ADTEINVNAM/FAO=1/WARNING-
<!AC violates the rules for DATATRIEVE names.!/Please enter a valid name. >
.BASE 440
ADTEYESNO/FAO=0/WARNING-
<Please answer with either YES (Y) or NO (N). >
.BASE 441
ADTFCHAR/FAO=6/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) !SL character!%S >
.BASE 442
ADTFDATE/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) is a date >
.BASE 443
ADTFGROUP/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) >
.BASE 444
ADTFMONEY/FAO=6/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) !SL digit!%S left (and 2 digits right) of the decimal, is money >
.BASE 445
ADTFNUMBERS/FAO=7/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) !SL digit!%S left and !SL digit!%S right of the decimal >
.BASE 446
ADTFPERCENT/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) is a percent >
.BASE 447
ADTIDEF/FAO=3/INFORMATIONAL-
<!/!/The DATATRIEVE definitions for your domain are!/located in file !AD!/The record length is !SL bytes. >
.BASE 448
ADTENOHELP/FAO=0/WARNING-
<No HELP text has been supplied for this state. >
.BASE 449
ADTENOPROMPT/FAO=0/WARNING-
<No prompt text has been supplied for this state. >
.BASE 450
SHOSEMI/FAO=0/INFORMATIONAL-
<    Semicolon >
.BASE 451
SHONOSEMI/FAO=0/INFORMATIONAL-
<    No semicolon >
.BASE 452
GUINOWORD/FAO=0/INFORMATIONAL-
<Can't complete word >
.BASE 453
ADTIBACKUP/FAO=0/INFORMATIONAL-
<ADT backing up one state >
.BASE 454
ADTIVIDEO/FAO=0/INFORMATIONAL-
" A D T   ? - Help   ! - Fields   < - Back up   <PF2> - Screen Help "
.BASE 455
CURRFAIL/FAO=1/ERROR-
<Record from source !AC failed to set up currency for set connection. >
.BASE 456
NOTINIMAG/FAO=0/INFORMATIONAL-
<That DATATRIEVE feature was not included when this image was built. >
.BASE 457
GUINOLINE/FAO=0/INFORMATIONAL-
<Can't complete line >
.BASE 458
OLDPLOT/FAO=2/ERROR-
<The plot !AD is stored in an incompatible format.  You must extract and re-define it. >
.BASE 459
PLONUMARG/FAO=0/WARNING-
<Wrong number of arguments supplied to internal plot call. >
.BASE 460
GPRFLOAT/FAO=0/INFORMATIONAL-
<numeric field >
.BASE 461
GPRALPHA/FAO=0/INFORMATIONAL-
<alphanumeric field >
.BASE 462
GPRDATE/FAO=0/INFORMATIONAL-
<date field >
.BASE 463
GPRRETURN/FAO=0/INFORMATIONAL-
"<return> "
.BASE 464
NOTSYNONYM/FAO=2/ERROR-
<"!AD" is not the name of a synonym. >
.BASE 465
FORMS/FAO=0/INFORMATIONAL-
<Loaded forms: >
.BASE 466
NOFORMS/FAO=0/INFORMATIONAL-
<No loaded forms. >
.BASE 467
FORMINLIB/FAO=6/INFORMATIONAL-
<    Form !AD in !AD ( !AD ) >
.BASE 468
SYNONYMS/FAO=0/INFORMATIONAL-
<Synonyms: >
.BASE 469
SHOWSYN/FAO=4/INFORMATIONAL-
<    !AD  !_for  !AD >
.BASE 470
RMSKEYUSE/FAO=3/INFORMATIONAL-
<Performing !AC boolean on RMS key field !AD >
.BASE 471
CLAIGNORE/FAO=1/WARNING-
<!AC clause is ignored when you use a COMPUTED BY clause. >
.BASE 472
COMFILNES/FAO=0/ERROR-
<Command file nesting limit exceeded. >
.BASE 473
CLAIGNGRP/FAO=1/WARNING-
<!AC clause ignored in group field definition. >
.BASE 474
PLOTSAT/FAO=1/INFORMATIONAL-
<Plots from !AC: >
.BASE 475
RMSERROR/FAO=2/INFORMATIONAL-
<Error using RMS file "!AD". >
.BASE 476
RMSCHG/FAO=0/ERROR-
<You cannot modify the value of an RMS key field that doesn't allow changes. >
.BASE 477
RMSDUP/FAO=0/ERROR-
<You cannot store a duplicate value in an RMS key field that doesn't allow duplicates. >
.BASE 478
BADSORLEN/FAO=1/ERROR-
<You tried to sort by too large a value or too many values.  (Total size: !UW) >
.BASE 479
NOLISTKEY/FAO=2/ERROR-
<Field "!AD" is part of an OCCURS list, so it cannot be used as a key. >
.BASE 480
BADQUOSTR/FAO=0/ERROR-
<Expected closing quote, encountered "***END_OF_LINE***". >
.BASE 481
NOFMI/FAO=0/ERROR-
<There is no forms package available. >
.BASE 482
GPRATTOP/FAO=0/INFORMATIONAL-
<AT TOP OF or PRINT >
.BASE 483
GPRATBOT/FAO=0/INFORMATIONAL-
<AT BOTTOM OF or END-REPORT >
.BASE 484
GPRFLDPAG/FAO=0/INFORMATIONAL-
<Field name or PAGE >
.BASE 485
GPRPRINT/FAO=0/INFORMATIONAL-
<PRINT >
.BASE 486
FMSITT/FAO=0/ERROR-
<Invalid terminal type for forms. >
.BASE 488
GUINUMBER/FAO=0/INFORMATIONAL-
"<a number> "
.BASE 489
SHOWEDIT/FAO=0/INFORMATIONAL-
<Edit setting: >
.BASE 490
EDITBACK/FAO=0/INFORMATIONAL-
<    Edit backup >
.BASE 491
EDITNOBACK/FAO=0/INFORMATIONAL-
<    Edit no backup >
.BASE 492
RMSLKWAIT/FAO=0/INFORMATIONAL-
<Waiting for a locked RMS record... >
.BASE 493
SHOLCKWAT/FAO=0/INFORMATIONAL-
<    Lock wait >
.BASE 494
SHONOLCKWAT/FAO=0/INFORMATIONAL-
<    No lock wait >
.BASE 495
ILLASCIICHAR/FAO=1/WARNING-
<Illegal ASCII character "!AS". >
.BASE 496
LOTOVEFLO/FAO=0/SEVERE-
<Internal error (no more slots available in lock table). >
.BASE 497
SELRECCLE/FAO=0/INFORMATIONAL-
<SELECT failed due to deadlock. >
.BASE 498
SELRECLOS/FAO=0/INFORMATIONAL-
<Failed to retrieve a seleted record following a re-ready. >
.BASE 499
NOMORESEL/FAO=1/INFORMATIONAL-
<Collection !AC will no longer have a selected record. >
.BASE 500
LOSSOFPREC/FAO=0/INFORMATIONAL-
<Loss of precision during arithmetic calculation. >
.BASE 501
REMSELBOO/FAO=0/ERROR-
<SELECT with a boolean is not supported for remote collections. >
.BASE 502
GHESET/FAO=0/INFORMATIONAL-
<Set default dictionary or plot dictionary >
.BASE 503
NODATA/FAO=1/ERROR-
<Field "!AD" from view cannot be used as a value. >
.BASE 504
BADGUICHA/FAO=0/ERROR-
<Invalid channel number for Guide Mode. >
.BASE 505
BADGUITT/FAO=0/ERROR-
<Invalid terminal type for Guide Mode. >
.BASE 506
HELPCONTIN/FAO=0/INFORMATIONAL-
"Type <RETURN> to continue. "
.BASE 507
NOFORMFLD/FAO=0/ERROR-
<Field does not exist on form >
.BASE 508
GPRALLVALRET/FAO=0/INFORMATIONAL-
"ALL, value expression or <return> "
.BASE 509
GPRATPRIEND/FAO=0/INFORMATIONAL-
<AT control break, PRINT or END_REPORT >
.BASE 510
GPRBREAK/FAO=0/INFORMATIONAL-
<PAGE, REPORT or sorted field name >
.BASE 511
GPRBY/FAO=0/INFORMATIONAL-
<BY sort field list >
.BASE 512
GPRDECINCFLD/FAO=0/INFORMATIONAL-
<DECREASING, INCREASING or field name >
.BASE 513
GPRFILNAM/FAO=0/INFORMATIONAL-
<file name >
.BASE 514
GPRNUMRET/FAO=0/INFORMATIONAL-
"number, end with a <space> or <return> "
.BASE 515
GPROF/FAO=0/INFORMATIONAL-
<OF control break >
.BASE 516
GPRON/FAO=0/INFORMATIONAL-
"ON filename or <return> "
.BASE 517
GPROPCOMRET/FAO=0/INFORMATIONAL-
"one of +, -, *, /, comma or <return> "
.BASE 518
GPROPRET/FAO=0/INFORMATIONAL-
"one of +, -, *, / or <return> "
.BASE 519
GPROPPARRET/FAO=0/INFORMATIONAL-
"one of +, -, *, /, ) or <return> "
.BASE 520
GPRSETOPT/FAO=0/INFORMATIONAL-
<set option >
.BASE 521
GPRSHOWOPT/FAO=0/INFORMATIONAL-
<show option >
.BASE 522
GPRTOPBOT/FAO=0/INFORMATIONAL-
<TOP or BOTTOM OF control break >
.BASE 523
GPRVALRET/FAO=0/INFORMATIONAL-
"value expression or <return> "
.BASE 524
GPRVALUE/FAO=0/INFORMATIONAL-
<value expression >
.BASE 525
GHEAT/FAO=0/INFORMATIONAL-
<Print values when the page or a field value changes >
.BASE 526
GHEBOTTOM/FAO=0/INFORMATIONAL-
<Print values at the end of a page or field value >
.BASE 527
GHEBY/FAO=0/INFORMATIONAL-
<Required word in the language >
.BASE 528
GHECDDPATH/FAO=0/INFORMATIONAL-
<Dictionary names (A-Z, 0-9, _, $) separated with periods >
.BASE 529
GHECOMMA/FAO=0/INFORMATIONAL-
<Comma to separate elements >
.BASE 530
GHEDECR/FAO=0/INFORMATIONAL-
<Sort field from largest to smallest >
.BASE 531
GHEDICT/FAO=0/INFORMATIONAL-
<Change the dictionary for your domains and procedures >
.BASE 532
GHEENDREP/FAO=0/INFORMATIONAL-
<Finish the report statement >
.BASE 533
GHEEQUAL/FAO=0/INFORMATIONAL-
<Equals >
.BASE 534
GHEFILNAM/FAO=0/INFORMATIONAL-
<Filename or device for output >
.BASE 535
GHEFIRST/FAO=0/INFORMATIONAL-
<First record of the collection >
.BASE 536
GHEINCR/FAO=0/INFORMATIONAL-
<Sort field from smallest to largest >
.BASE 537
GHELAST/FAO=0/INFORMATIONAL-
<Last record of the collection >
.BASE 538
GHELFPAREN/FAO=0/INFORMATIONAL-
<Parenthesized value expression >
.BASE 539
GHELITERAL/FAO=0/INFORMATIONAL-
<Quoted string >
.BASE 540
GHENEWLINE/FAO=0/INFORMATIONAL-
<Finish the current line of the report >
.BASE 541
GHENOT/FAO=0/INFORMATIONAL-
<Reverse the sense of the comparison >
.BASE 542
GHENUMBER/FAO=0/INFORMATIONAL-
<A number >
.BASE 543
GHEOF/FAO=0/INFORMATIONAL-
<Required word in the language >
.BASE 544
GHEON/FAO=0/INFORMATIONAL-
<Send report output to �                                                                                                                                                                                                                                                                           W�N� $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         �� "     R       a file or device >
.BASE 545
GHEOPER/FAO=0/INFORMATIONAL-
<Arithmetic operator:  +, -, * or / >
.BASE 546
GHEPAGE/FAO=0/INFORMATIONAL-
<Print values at the start or end of the page >
.BASE 547
GHEPLOTS/FAO=0/INFORMATIONAL-
<Change the dictionary for your plots >
.BASE 548
GHEQUOTE/FAO=0/INFORMATIONAL-
<Any character, end with a quote >
.BASE 549
GHEREPORT/FAO=0/INFORMATIONAL-
<Create a formatted report >
.BASE 550
GHEREPPRI/FAO=0/INFORMATIONAL-
<Print values for every record >
.BASE 551
GHEREPREP/FAO=0/INFORMATIONAL-
<Print values at the start or end of the report >
.BASE 552
GHERTPAREN/FAO=0/INFORMATIONAL-
<Closing parenthesis >
.BASE 553
GHESHOCOL/FAO=0/INFORMATIONAL-
<Display available collections >
.BASE 554
GHESHOCUR/FAO=0/INFORMATIONAL-
<Display details about the current collection >
.BASE 555
GHESHODOM/FAO=0/INFORMATIONAL-
<Display available domains >
.BASE 556
GHESHOPLO/FAO=0/INFORMATIONAL-
<Display available plots >
.BASE 557
GHESHOPRO/FAO=0/INFORMATIONAL-
<Display available procedures >
.BASE 558
GHESHORDY/FAO=0/INFORMATIONAL-
<Display details about readied domains >
.BASE 559
GHESTART/FAO=0/INFORMATIONAL-
<Starting with a substring >
.BASE 560
GHETOP/FAO=0/INFORMATIONAL-
<Print values at the start of a page or field value >
.BASE 561
GPRCDDPATH/FAO=0/INFORMATIONAL-
<dictionary path name >
.BASE 562
GHEAVERAGE/FAO=0/INFORMATIONAL-
<Average of a field >
.BASE 563
GHECOUNT/FAO=0/INFORMATIONAL-
<Number of records >
.BASE 564
GHESTDDEV/FAO=0/INFORMATIONAL-
<Standard deviation of a field >
.BASE 565
GHETOTAL/FAO=0/INFORMATIONAL-
<Sum of a field >
.BASE 566
GHERUNNING/FAO=0/INFORMATIONAL-
<Compute statistical for each record >
.BASE 567
GPRRUNNING/FAO=0/INFORMATIONAL-
<running statistical >
.BASE 568
GHEMAX/FAO=0/INFORMATIONAL-
<Maximum value of a field >
.BASE 569
GHEMIN/FAO=0/INFORMATIONAL-
<Minimum value of a field >
.BASE 570
GHECOMP/FAO=0/INFORMATIONAL-
"<space> "
.BASE 571
GHETERM/FAO=0/INFORMATIONAL-
"<return> "
.BASE 572
GHECHAR/FAO=0/INFORMATIONAL-
"<any character> "
.BASE 573
GPRALL/FAO=0/INFORMATIONAL-
<ALL value expression list >
.BASE 574
GPRPLOTARG/FAO=0/INFORMATIONAL-
<!AC >
.BASE 575
GUIWORKING/FAO=0/INFORMATIONAL-
<[Working...] >
.BASE 576
RDSSIGNON/FAO=2/INFORMATIONAL-
<DEC DATATRIEVE Remote Server!/Protocol !UL.!UL >
.BASE 577
RDSSIGNOFF/FAO=1/INFORMATIONAL-
<DEC DATATRIEVE Remote Server terminating with status !XL >
.BASE 578
REMCROSS/FAO=0/ERROR-
<CROSS can only be used on domains with the same node specification. >
.BASE 579
REMNOCROSS/FAO=0/ERROR-
<This remote node does not support CROSS. >
.BASE 580
SOURCE/FAO=0/INFORMATIONAL-
<Ready sources: >
.BASE 581
NOFLDS/FAO=0/ERROR-
<No field information available >
.BASE 582
RDBRB/FAO=3/INFORMATIONAL-
<Relational database !AD on !AS; >
.BASE 583
NOALIAS/FAO=0/ERROR-
<You can not READY a database with an alias. >
.BASE 585
CANRDYSRC/FAO=0/ERROR-
<Source cannot be readied unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 586
DICNOTRED/FAO=0/ERROR-
<Dictionaries may not be redefined. >
.BASE 587
FILNOTRED/FAO=0/ERROR-
<Files may not be redefined. >
.BASE 588
REDEFINE/FAO=0/WARNING-
<Element to be redefined not found in dictionary - new element defined. >
.BASE 589
NOTRELNAM/FAO=4/ERROR-
<"!AD" is not a valid relation name for database "!AD". >
.BASE 590
TRAPURCOL/FAO=2/WARNING-
<New relational transaction started; collection !AD automatically released. >
.BASE 594
BADFLDTYP/FAO=2/WARNING-
<Field "!AD" ignored because of unsupported datatype. >
.BASE 596
SNAPTOSHARE/FAO=0/WARNING-
<Some record sources currently READYed SNAPSHOT will now be accessed SHARED READ. >
.BASE 597
RDYSNPTOSHR/FAO=0/INFORMATIONAL-
<READY SNAPSHOT (READ) mode has been changed to READY SHARED READ mode for the current RMS record source. >
.BASE 598
COMROLLCOMP/FAO=0/INFORMATIONAL-
<COMMIT / ROLLBACK completed. >
.BASE 599
RDBCOMMITSTA/FAO=0/WARNING-
<Statement containing COMMIT aborted after first COMMIT. >
.BASE 600
RDBCOMPOUND/FAO=0/ERROR-
<Compound statements referencing relational fields may not contain COMMITs. >
.BASE 601
RDBREQSYNC/FAO=0/ERROR-
<Statement aborted by relational database due to an invalid request for data. >
.BASE 602
RECEXCMAX/FAO=0/ERROR-
<Record size exceeds maximum. >
.BASE 603
ROLPURCOL/FAO=2/WARNING-
<ROLLBACK executed; collection !AD automatically released. >
.BASE 604
PURKEEPBAD/FAO=0/ERROR-
<The KEEP value for PURGE must be greater than zero. >
.BASE 605
PURELMNOT/FAO=1/WARNING-
<No objects purged for dictionary element "!AS". >
.BASE 606
PURDICNOT/FAO=0/WARNING-
<No objects purged for the current default dictionary. >
.BASE 607
PURELMSEM/FAO=1/ERROR-
<You cannot purge a specific version of object "!AS". >
.BASE 608
PURNOTFND/FAO=1/WARNING-
<Dictionary element "!AS" not found. >
.BASE 609
PURNOTDTR/FAO=1/WARNING-
<Element "!AS" is not a Domain, Record, Procedure, or Table. >
.BASE 610
DELNOPRIV/FAO=1/WARNING-
<No privilege to delete object "!AS". >
.BASE 611
SHOCOLREL/FAO=1/INFORMATIONAL-
<    Relation: !AC >
.BASE 612
SHOCOLREC/FAO=1/INFORMATIONAL-
<    Record: !AC >
.BASE 613
RDBNOREL/FAO=2/ERROR-
<Database "!AD" cannot be readied since it contains no relations. >
.BASE 614
BADSEGEDIT/FAO=2/WARNING-
<Segmented string field "!AD" must have "T" edit string, defaulting to T(COLUMNS_PAGE). >
.BASE 615
NOSEGDEFAULT/FAO=2/WARNING-
<Segmented string field "!AD" cannot have a default value, default value will be ignored. >
.BASE 616
NOSEGMISSING/FAO=2/WARNING-
<Segmented string field "!AD" cannot have a missing value, missing value will be ignored. >
.BASE 617
SEGWRITERROR/FAO=2/WARNING-
<Error writing to segmented string field "!AD". >
.BASE 618
SEGCANCERROR/FAO=2/WARNING-
<Error canceling segmented string created for field "!AD". >
.BASE 619
SEGCLOSERROR/FAO=2/WARNING-
<Error closing segmented string for field "!AD". >
.BASE 620
SEGREADERROR/FAO=2/WARNING-
<Error reading segmented string from field "!AD". >
.BASE 621
SEGCREAERROR/FAO=2/WARNING-
<Error creating segmented string for output to field "!AD". >
.BASE 622
SEGOPENERROR/FAO=2/WARNING-
<Error opening segmented string from field "!AD". >
.BASE 623
NOTSEGMENT/FAO=2/SEVERE-
<"!AD" is not a segmented string field. >
.BASE 624
NOSEGASS/FAO=2/WARNING-
<Ignoring illegal assignment to or from segmented string field "!AD". >
.BASE 625
GUITOOMNY/FAO=0/ERROR-
<The current domain has too many fields for GUIDE MODE to continue. >
.BASE 626
ILLSEGOPR/FAO=2/ERROR-
<Statement is illegal for segmented string field "!AD". >
.BASE 627
ASSERROR/FAO=0/WARNING-
<Error occurred during assignment. >
.BASE 628
BADASSIGN/FAO=2/WARNING-
<Variable "!AD" may contain an incorrect value due to error during assignment. >
.BASE 629
BADMODSTO/FAO=2/WARNING-
<Field "!AD" may contain an incorrect value due to error during STORE or MODIFY. >
.BASE 630
RDBERRFIN/FAO=0/WARNING-
<Unable to start a relational database transaction, finishing all relational domains. >
.BASE 633
CHGSNAPAC/FAO=2/WARNING-
<SNAPSHOT WRITE is not a valid access mode.  Readying "!AD" SNAPSHOT READ. >
.BASE 634
BADCONSIST/FAO=0/ERROR-
<It is illegal to have CONSISTENCY and CONCURRENCY specified for the same database. >
.BASE 635
NOCONSIST/FAO=3/WARNING-
<Ignoring "!AC" specified for non relational database source "!AD". >
.BASE 636
DIFFERENTDB/FAO=2/WARNING-
<"!AD" cannot be readied with the same name as a currently readied source. >
.BASE 637
NOSNAPSDBM/FAO=1/INFORMATIONAL-
<"!AC" cannot be accessed in SNAPSHOT mode.  Changing mode to SHARED READ. >
.BASE 638
NOTRDYSNAP/FAO=1/WARNING-
<DBMS source does not permit snapshots. "!AC" cannot be readied SNAPSHOT. >
.BASE 639
NOSNAPCOM/FAO=0/WARNING-
<A COMMIT cannot be done while there is a DBMS source readied in SNAPSHOT mode. >
.BASE 640
INVEDITOR/FAO=1/ERROR-
<!AS is not a valid editor within DATATRIEVE. >
.BASE 641
NOCODTRAN/FAO=0/ERROR-
<Table must include a code/translation pair. >
.BASE 642
RECNOTOPT/FAO=0/WARNING-
<Record will be defined, but will not be optimized. >
.BASE 643
PLOHDRLNG/FAO=0/ERROR-
<A title line for a plot cannot exceed 49 characters. >
.BASE 644
RDYNOTOPT/FAO=1/WARNING-
<Use of optimization for record "!AC" failed.  Processing will continue. >
.BASE 645
DROPOPT/FAO=2/WARNING-
<Keyword "OPTIMIZE" removed from definition of record "!AD". >
.BASE 646
RECNOTCUR/FAO=1/WARNING-
<Record "!AC" uses old record format.  Processing will!/continue, but for optimization you must redefine record. >
.BASE 647
STACKSIZE/FAO=1/INFORMATIONAL-
<DEC DATATRIEVE stack size is !UL. >
.BASE 648
EXTBADPARAM/FAO=0/ERROR-
<Argument list cannot contain both the object path name and the object type. >
.BASE 649
BADRDYMODE/FAO=2/WARNING-
<The value of DTR$READY_MODE, "!AD", is not a valid access option. >
.BASE 650
RDYMODIGN/FAO=0/WARN�                                                                                                                                                                                                                                                                           v5j; $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         UX "     c       ING-
<!/The preceding error occurred while translating DTR$READY_MODE.!/Therefore, the DATATRIEVE default access option will be used. >
.BASE 651
PLOTOOBIG/FAO=0/ERROR-
<Maximum plot size exceeded. >
.BASE 652
BADEDIT/FAO=0/ERROR-
<Error encountered when entering editor. >
.BASE 653
REINITCDD/FAO=0/INFORMATIONAL-
<Reinitializing CDD >
.BASE 654
CDDCORRUPT/FAO=0/ERROR-
<CDD could not be reinitialized.  Exit and reinvoke DATATRIEVE!/before attempting to access the CDD. >
.BASE 655
ILLSEGEDIT/FAO=2/ERROR-
<Segmented string field "!AD" must have a "T" (text) edit string. >
.BASE 656
BADKEYTBL/FAO=0/ERROR-
<Invalid key table ID. >
.BASE 657
BADKEYBRD/FAO=0/ERROR-
<Invalid key board ID. >
.BASE 658
KEYSTATE/FAO=1/INFORMATIONAL-
<!AS state keypad definitions: >
.BASE 659
KEYDEF/FAO=1/INFORMATIONAL-
<    !AS = "!AS" >
.BASE 660
KEYATRBTS/FAO=6/INFORMATIONAL-
<        (!ACecho,!ACterminate,!ACerase,!AClock!AC!AS) >
.BASE 661
INVKEYMOD/FAO=1/WARNING-
<"!AS" is not a valid alternate keypad mode. >
.BASE 662
FDLERROR/FAO=1/ERROR-
<Error using FDL file "!AD". >
.BASE 663
FDLSTMNT/FAO=2/ERROR-
<Error using FDL file "!AD"; statement !UL. >
.BASE 664
FDLOPEN/FAO=1/ERROR-
<Error opening FDL file "!AD". >
.BASE 665
FDLBADSIZ/FAO=2/ERROR-
<Incompatible record size.  Defined: !UW FDL File: !UW >
.BASE 666
EXCCMPSTK/FAO=1/ERROR-
<Exceeded compiler stack level at level !UL. >
.BASE 667
UNSHOWDB/FAO=2/WARNING-
<Unable to show !AD relational database. >
.BASE 668
APPKEYPAD/FAO=0/INFORMATIONAL-
<    Application keypad mode >
.BASE 669
NUMKEYPAD/FAO=0/INFORMATIONAL-
<    Numeric keypad mode >
.BASE 670
BADSIGN/FAO=1/WARNING-
<Invalid sign or signs in string "!AS". >
.BASE 671
FUNCAPTIVE/FAO=0/ERROR-
<Function cannot be executed from a captive account. >
.BASE 672
INFBADIDX/FAO=0/ERROR-
<Information index is out of range. >
.BASE 673
RELDBSHOW/FAO=2/INFORMATIONAL-
<Relational database !AD : >
.BASE 674
MAXSCRLIN/FAO=0/ERROR-
<Maximum lines for scrolling region exceeded. >
.BASE 675
INVBLKSIZ/FAO=1/SEVERE-
<Internal error (attempted to get a block of invalid size: !XL). >
.BASE 676
RELINK/FAO=0/ERROR-
<Please relink your DATATRIEVE image or reinstall DATATRIEVE >
.BASE 677
MAXEXTEXC/FAO=0/ERROR-
<Maximum size for extract or edit buffer exceeded. >
.BASE 678
CDDONLY/FAO=1/WARNING-
<You cannot perform this operation on CDO dictionary node, "!AS". >
.BASE 679
INVBUF/FAO=0/SEVERE-
<Internal error (invalid CDD/Plus metadata buffer). >
.BASE 680
DEFLIST/FAO=1/INFORMATIONAL-
<                       = !AC >
.BASE 681
NOGOCDO/FAO=1/ERROR-
<A DATATRIEVE !AC cannot be stored in a CDO directory. >
.BASE 682
DROPRELAT/FAO=2/WARNING-
<RELATIONSHIPS cannot be applied to DMU objects.!/The RELATIONSHIPS qualifier has been removed from domain "!AD". >
.BASE 683
RELONLYCDO/FAO=0/WARNING-
<RELATIONSHIPS will be applied only to CDO objects. >
.BASE 684
PASTTOP/FAO=0/ERROR-
<You used too many hyphens in a path name. >
.BASE 685
NOWINDOWS/FAO=0/ERROR-
<The DATATRIEVE window interface is not active. >
.BASE 686
NOCOMMAND/FAO=0/INFORMATIONAL-
<DATATRIEVE is not ready to process a command or statement. >
.BASE 687
INVOPTION/FAO=0/ERROR-
<An invalid option has been specified for the DTR$WINDOWS call. >
.BASE 688
NOSMG/FAO=0/ERROR-
<The DATATRIEVE SMG keyboard interface is not active. >
.BASE 689
PRINTDAB/FAO=2/INFORMATIONAL-
<!AD >
.BASE 690
NOSMGINFO/FAO=0/INFORMATIONAL-
<The DATATRIEVE SMG keyboard interface is not active. >
.BASE 691
MESSAGE/FAO=2/INFORMATIONAL-
<!AD >
.BASE 692
OUTPUT/FAO=2/INFORMATIONAL-
<!AD >
.BASE 693
FATAL/FAO=0/SEVERE-
<Unrecoverable window interface error, performing image exit. >
.BASE 694
BADFETCH/FAO=0/ERROR-
<Unable to fetch an object from the window interface DRM database. >
.BASE 695
CSERROR/FAO=0/ERROR-
<Windows interface compound string conversion error. >
.BASE 696
XLIBERR/FAO=0/INFORMATIONAL-
<Error received from XLIB : >
.BASE 697
INVOKEFILCOM/FAO=1/INFORMATIONAL-
<@!AS; >
.BASE 698
CLOSEFILCOM/FAO=0/INFORMATIONAL-
<CLOSE; >
.BASE 699
EXITCOM/FAO=0/INFORMATIONAL-
<EXIT; >
.BASE 700
SHOWREADYCOM/FAO=0/INFORMATIONAL-
<SHOW READY; >
.BASE 701
SHOWFIELDSCM/FAO=0/INFORMATIONAL-
<SHOW FIELDS; >
.BASE 702
SHOWCURRENT/FAO=0/INFORMATIONAL-
<SHOW CURRENT; >
.BASE 703
SHOWCOLLECTS/FAO=0/INFORMATIONAL-
<SHOW COLLECTIONS; >
.BASE 704
SHOWVARIABLE/FAO=0/INFORMATIONAL-
<SHOW VARIABLES; >
.BASE 705
SHOWSETSCOM/FAO=0/INFORMATIONAL-
<SHOW SETS; >
.BASE 706
OPENLOGFILE/FAO=1/INFORMATIONAL-
<OPEN !AS; >
.BASE 707
SHOWFIELDSFR/FAO=1/INFORMATIONAL-
<SHOW FIELDS FOR !AS; >
.BASE 708
SHOWNAMEDCOM/FAO=1/INFORMATIONAL-
<SHOW !AS; >
.BASE 709
SETCOLSPAGE/FAO=1/INFORMATIONAL-
<SET COLUMNS_PAGE = !AS; >
.BASE 710
READYSOURCE/FAO=5/INFORMATIONAL-
<READY !AS !AS !AS !AS !AS; >
.BASE 711
EXTRACTOBJ/FAO=2/INFORMATIONAL-
<EXTRACT !AS !AS; >
.BASE 712
SETABORTCOM/FAO=0/INFORMATIONAL-
<SET ABORT; >
.BASE 713
SETNOABORT/FAO=0/INFORMATIONAL-
<SET NO ABORT; >
.BASE 714
SETLOCKWAIT/FAO=0/INFORMATIONAL-
<SET LOCK_WAIT; >
.BASE 715
SETNOLOCKWAI/FAO=0/INFORMATIONAL-
<SET NO LOCK_WAIT; >
.BASE 716
SETPROMPTCOM/FAO=0/INFORMATIONAL-
<SET PROMPT; >
.BASE 717
SETNOPROMPT/FAO=0/INFORMATIONAL-
<SET NO PROMPT; >
.BASE 718
SETSEARCH/FAO=0/INFORMATIONAL-
<SET SEARCH; >
.BASE 719
SETNOSEARCH/FAO=0/INFORMATIONAL-
<SET NO SEARCH; >
.BASE 720
SETSEMICOLON/FAO=0/INFORMATIONAL-
<SET SEMICOLON; >
.BASE 721
SETNOSEMICOL/FAO=0/INFORMATIONAL-
<SET NO SEMICOLON; >
.BASE 722
SETVERIFY/FAO=0/INFORMATIONAL-
<SET VERIFY; >
.BASE 723
SETNOVERIFY/FAO=0/INFORMATIONAL-
<SET NO VERIFY; >
.BASE 724
RSNAPSHOT/FAO=0/INFORMATIONAL-
<SNAPSHOT >
.BASE 725
RPROTECTED/FAO=0/INFORMATIONAL-
<PROTECTED >
.BASE 726
RSHARED/FAO=0/INFORMATIONAL-
<SHARED >
.BASE 727
REXCLUSIVE/FAO=0/INFORMATIONAL-
<EXCLUSIVE >
.BASE 728
RREAD/FAO=0/INFORMATIONAL-
<READ >
.BASE 729
RMODIFY/FAO=0/INFORMATIONAL-
<MODIFY >
.BASE 730
RWRITE/FAO=0/INFORMATIONAL-
<WRITE >
.BASE 731
REXTEND/FAO=0/INFORMATIONAL-
<EXTEND >
.BASE 732
RCONSISTENCY/FAO=0/INFORMATIONAL-
<CONSISTENCY >
.BASE 733
RCONCURRENCY/FAO=0/INFORMATIONAL-
<CONCURRENCY >
.BASE 734
NOGODMU/FAO=0/ERROR-
<Definition contains attributes that cannot be stored in DMU dictionary. >
.BASE 735
NOSELECTMEM/FAO=0/ERROR-
<Unable to allocate memory for CUT or COPY or PASTE selection. >
.BASE 736
NOSELECTION/FAO=0/ERROR-
<No current selection exists. >
.BASE 737
CLIPFAIL/FAO=0/ERROR-
<Failure to access DECwindows clipboard. >
.BASE 738
CLIPTRUNCATE/FAO=0/ERROR-
<DECwindows clipboard data truncated. >
.BASE 739
CLIPLOCKED/FAO=0/ERROR-
<The DECwindows clipboard is locked, please try again later. >
.BASE 740
CLIPNODATA/FAO=0/ERROR-
<No data is currently contained on the DECwindows clipboard. >
.BASE 741
PASTELENGTH/FAO=0/ERROR-
<Item to be pasted is too large for text field. >
.BASE 742
RELMSG/FAO=8/WARNING-
<"!AD" is related to an invalid "!AD", triggered by !/!AD entity "!AD". >
.BASE 743
POSMSG/FAO=6/WARNING-
<"!AD" is possibly invalid, triggered by !/!AD entity "!AD". >
.BASE 744
INVMSG/FAO=6/WARNING-
<"!AD" is invalid, triggered by !/!AD entity "!AD". >
.BASE 745
CHILDMSG/FAO=6/WARNING-
<"!AD" is related to an entity whose relationships have !/changed, triggered by !AD entity "!AD". >
.BASE 746
NOMESSAGES/FAO=2/INFORMATIONAL-
<"!AD" has no messages. >
.BASE 747
NEWVERMSG/FAO=6/WARNING-
<"!AD" uses an entity which has new versions, triggered by !/!AD entity "!AD". >
.BASE 748
UNKMSG/FAO=6/WARNING-
<"!AD" has a message of unknown type, triggered by !/!AD entity "!AD". >
.BASE 749
MSGPROB/FAO=2/WARNING-
<An error occured while fetching CDD/Plus messages for "!AD". >
.BASE 750
SHOWALL/FAO=0/INFORMATIONAL-
<SHOW ALL; >
.BASE 751
SHOWDICT/FAO=0/INFORMATIONAL-
<SHOW DICTIONARIES; >
.BASE 752
SHOWDOMAINS/FAO=0/INFORMATIONAL-
<SHOW DOMAINS; >
.BASE 753
SHOWPROCED/FAO=0/INFORMATIONAL-
<SHOW PROCEDURES; >
.BASE 754
SHOWRECS/FAO=0/INFORMATIONAL-
<SHOW RECORDS; >
.BASE 755
SHOWTABS/FAO=0/INFORMATIONAL-
<SHOW TABLES; >
.BASE 756
SHOWDICTION/FAO=0/INFORMATIONAL-
<SHOW DICTIONARY; >
.BASE 757
UNKNOWN/FAO=0/INFORMATIONAL-
<UNKNOWN >
.BASE 758
INVOKEADT/FAO=0/INFORMATIONAL-
<ADT; >
.BASE 759
SETGUIDE/FAO=0/INFORMATIONAL-
<SET GUIDE; >
.BASE 760
BADRELNAM/FAO=2/WARNING-
<Invalid relation name.  "!AD" will be used and the remainder dropped. >
.BASE 761
EDITCOMMAND/FAO=0/INFORMATIONAL-
<EDIT; >
.BASE 762
NODECTERM/FAO=0/ERROR-
<Unable to create a DECterm window - execute DTR$LIBRARY:REMOTE_DECTERM.COM for remote displays. >
.BASE 763
NOTFLDNAM/FAO=1/ERROR-
<"!AS" is not a field name. >
.BASE 764
NOTALLREL/FAO=0/WARNING-
<Some domains referenced by view were defined without RELATIONSHIPS.  Therefore!/not all view relationships will be formed. >
.BASE 765
OVFMETABF/FAO=2/SEVERE-
<Internal error (Meta data buffer overflow, length = !U�                                                                                                                                                                                                                                                                           �#�� $      
DTRJA061.C                     �   
  [C]DTRMSGS.MSG;1                                                                                                                     �                         ��       t       L, used = !UL). >
.BASE 766
DISMISSWORK/FAO=0/INFORMATIONAL-
<Dismiss work in progress box. >
.BASE 767
MEMSTUCK/FAO=0/SEVERE-
<Internal error (virtual memory could not be released). >
.BASE 768
NEWDOMVER/FAO=2/INFORMATIONAL-
<DEFINE FILE parameters conflict with previous values.  Therefore!/a new version of "!AD" has been created. >
.BASE 769
INVACLATT/FAO=2/ERROR-
<Invalid ACL attribute found for "!AD". >
.BASE 770
CDDNOTFOUND/FAO=1/ERROR-
<Element "!AS" cannot be found in the dictionary. >
.BASE 771
CDONOTFOUND/FAO=1/ERROR-
<Element "!AS" is not in the CDO dictionary. >
.BASE 772
PROTNOTSUPP/FAO=1/ERROR-
<Use CDO to manage protection for element "!AS". >
.BASE 773
NOPASSWORD/FAO=1/WARNING-
<Password ignored for CDO element "!AS". >
.BASE 774
NOTERMINAL/FAO=3/WARNING-
<TERMINAL option "!AD" ignored for CDO element "!AS". >
.BASE 775
NOBANISH/FAO=1/WARNING-
<BANISH option ignored for CDO element "!AS". >
.BASE 776
CDOIDENT/FAO=2/INFORMATIONAL-
<  !UB:!AS >
.BASE 777
CDOACCESS/FAO=1/INFORMATIONAL-
<        !AS >
.BASE 778
DTRACCESS/FAO=2/INFORMATIONAL-
<        Grant - !AS, Deny - !AS >
.BASE 779
NOGLOBDELETE/FAO=0/WARNING-
<G (GLOBAL_DELETE) protection option ignored for CDO dictionary. >
.BASE 780
NOPASSTHRU/FAO=0/WARNING-
<P (PASS_THRU) protection option ignored for CDO dictionary. >
.BASE 781
NODMUEXTEND/FAO=0/WARNING-
<X (EXTEND) protection option ignored for CDO dictionary. >
.BASE 782
NOFORWARD/FAO=0/WARNING-
<F (FORWARD) protection option ignored for CDO dictionary. >
.BASE 783
NOHISTORY/FAO=0/WARNING-
<H (HISTORY) protection option ignored for CDO dictionary. >
.BASE 784
INVDIRPROT/FAO=2/ERROR-
<Invalid ACL found for directory "!AD". >
.BASE 785
DIFFNODE/FAO=4/ERROR-
<Node name in remote domain anchor, "!AD", does not!/match name in node specification, "!AD". >
.BASE 786
NOGRANTDENY/FAO=1/ERROR-
<GRANT or DENY must be specified for "!AS". >
.BASE 787
BADSEQUENCE/FAO=0/WARNING-
<No ACL exists at sequence number position. >
.BASE 788
IGCMDARGS/FAO=2/WARNING-
<"!AD" is a CDD$DATABASE domain.  File defined according to!/domain attributes.  DEFINE FILE command line arguments ignored. >
.BASE 789
SHOWPROT/FAO=2/INFORMATIONAL-
<!AD >
.BASE 790
SHOPRIVRN/FAO=0/INFORMATIONAL-
< R (CDO READ)           - may ready for READ, use SHOW, EDIT and EXTRACT >
.BASE 791
SHOPRIVWN/FAO=0/INFORMATIONAL-
< W (CDO WRITE)          - may ready for READ, WRITE, MODIFY or EXTEND >
.BASE 792
SHOPRIVMN/FAO=0/INFORMATIONAL-
< M (CDO MODIFY)         - may ready for READ, MODIFY >
.BASE 793
SHOPRIVEN/FAO=0/INFORMATIONAL-
< E (CDO EXTEND+WRITE)   - may ready for EXTEND >
.BASE 794
SHOPRIVCN/FAO=0/INFORMATIONAL-
< C (CDO CONTROL)        - may issue DEFINEP, SHOWP, DELETEP commands >
.BASE 795
SHOPRIVDN/FAO=0/INFORMATIONAL-
< D (CDO DELETE)         - may delete a dictionary object or empty directory >
.BASE 796
SHOWPRIVSN/FAO=0/INFORMATIONAL-
< S (CDO SHOW)           - may see dictionary object >
.BASE 797
SHOPRIVUN/FAO=0/INFORMATIONAL-
< U (CDO DEFINE+CHANGE)  - may define or update dictionary object >
.BASE 798
RMSDBSHOW/FAO=8/INFORMATIONAL-
<CDD$DATABASE !AD uses!/    RMS_DATABASE : !AD !/    RECORD : !AD !/    FILE : !AD >
.BASE 799
INVOKEPROC/FAO=1/INFORMATIONAL-
<:!AS; >
.BASE 800
READYDEFAULT/FAO=1/INFORMATIONAL-
<READY !AS; >
.BASE 801
SETCDDDICT/FAO=1/INFORMATIONAL-
<SET DICTIONARY !AS; >
.BASE 802
BADELEMENTID/FAO=0/ERROR-
<Unable to access selected CDD element. >
.BASE 803
NONAVIGATOR/FAO=0/ERROR-
<Unable to invoke the CDD navigator. >
.BASE 804
CANTSPAWN/FAO=0/WARNING-
<Subprocess could not be spawned. >
.BASE 805
EDITOBJECT/FAO=1/INFORMATIONAL-
<EDIT !AS; >
.BASE 806
NOCDOCMD/FAO=0/ERROR-
<Expected CDO command, encountered end of line. >
.BASE 807
DECWOUTPUT/FAO=1/INFORMATIONAL-
<!AS >
.BASE 808
HLPACTIVE/FAO=0/INFORMATIONAL-
<Help window already active. >
.BASE 809
DMUFROMFLD/FAO=1/ERROR-
<DMU record "!AS" may not be referenced in FROM field. >
.BASE 810
DEFSCROLLIN/FAO=1/WARNING-
<Unable to allocate scroll lines specified, defaulting to !UL. >
.BASE 811
BADFLDEXP/FAO=2/WARNING-
<Field "!AD" ignored because of invalid expression. >
.BASE 812
IGNVALEXP/FAO=2/WARNING-
<Invalid VALID IF expression in field "!AD" will be ignored. >
.BASE 813
SENDMAINXY/FAO=0/INFORMATIONAL-
<Request to place main window X,Y position in DAB. >
.BASE 814
BADHLPINIT/FAO=0/WARNING-
<Problem encountered initializing HELP window. >
.BASE 815
COLSPAGE/FAO=0/INFORMATIONAL-
<The columns_page setting has changed. >
.BASE 816
SETFORMSCOM/FAO=0/INFORMATIONAL-
<SET FORM; >
.BASE 817
SETNOFORMS/FAO=0/INFORMATIONAL-
<SET NO FORM; >
.BASE 818
ILLOPERVIEW/FAO=1/ERROR-
<Illegal operation for relational view source "!AC". >
.BASE 819
BADSOURCE/FAO=2/ERROR-
<Object "!AD" is not a valid DATATRIEVE source. >
.BASE 820
BADLOGNAM/FAO=1/WARNING-
<Error found while processing logical name !AS >
.BASE 821
RESATTNAM/FAO=2/ERROR-
<"!AD" is a reserved attribute name >
.BASE 822
DROPUSING/FAO=2/WARNING-
<FORM IS...USING cannot be applied to DMU domains.!/The USING clause has been removed from domain "!AD" >
.BASE 823
DFLOADFORM/FAO=0/ERROR-
<Failure attempting to load a DECforms form. >
.BASE 824
DFNOLAYOUT/FAO=0/ERROR-
<No layouts in the DECforms form conform to this terminal type,language, and display size. >
.BASE 825
DFNOLICENSE/FAO=0/ERROR-
<DECforms form manager is present, but no DECforms software license is active >
.BASE 826
DFOPENFORM/FAO=2/ERROR-
<Error opening DECforms form file !AD . >
.BASE 827
DFREADFORM/FAO=2/ERROR-
<Error reading DECforms form file !AD . >
.BASE 828
DFBADRECCNT/FAO=0/ERROR-
<The number of DATATRIEVE records does not match the number of records in the form record list. >
.BASE 829
DFNORECORD/FAO=0/ERROR-
<A form record specified in the operation is unknown in the DECforms form. >
.BASE 830
DFDBLATTEMPT/FAO=0/INFORMATIONAL-
<Both DECforms and FMS/TDMS are unable to access the specified form >
.BASE 831
DFNOTNOW/FAO=0/ERROR-
<A DECforms form cannot be used now. >
.BASE 832
NOFORMAT/FAO=0/ERROR-
<Unable to load exchange record definition. >
.BASE 833
NOWITHF/FAO=0/ERROR-
<Is not possible to use the WITH_FORM statement when a SET NO FORM is active >
.BASE 834
NOFTREE/FAO=0/ERROR-
<Unable to build the Field Tree >
.BASE 835
NOEXCHANGE/FAO=0/ERROR-
<An EXCHANGE record is needed to perform this operation >
.BASE 836
CDANOTINST/FAO=0/ERROR-
<Aborting report execution- cannot access CDA image >
.BASE 837
NODELETE/FAO=1/WARNING-
<Could not delete temporary file !AS >
.BASE 838
CDAERROR/FAO=0/INFORMATIONAL-
<Aborting report execution due to CDA toolkit error. >
.BASE 839
FMTNEEDON/FAO=1/ERROR-
<Execution of a report in !AS format requires an ON clause specifying the file name. >
.BASE 840
SYNTAX_ERR/FAO=2/ERROR-
<Syntax error at or near "!AD" . >
.BASE 841
TOOMANYID/FAO=0/SEVERE-
<Internal error (exceeded identifier array size). >
.BASE 842
CDONOTRANS/FAO=0/ERROR-
<Unable to build Datatrieve source text from this CDO definition. >
.BASE 843
DFBADRECLEN/FAO=0/ERROR-
<The length of the DATATRIEVE record and the form record do not match. >
.BASE 844
COLTOOBIG/FAO=0/ERROR-
<Too many records for this collection >
.BASE 845
DTYPEUNS/FAO=1/ERROR-
<!AC data type is not supported on this platform. >
.BASE 846
FLDTYPCNV/FAO=2/WARNING-
<Field "!AD" converted to !AC because of unsupported datatype. >
.BASE 847
LITTYPCNV/FAO=1/WARNING-
<A literal was converted to !AC because of unsupported data type. >
.BASE 848
NOTRDB/FAO=2/ERROR-
<"!AD" is not a relation. >
.BASE 849
INVWDTHRANGE/FAO=2/ERROR-
<WIDTH value of !UL not in range for !AC paper orientation. >
.BASE 850
NOTELEST/FAO=2/ERROR-
<Non-elementary field !AD cannot be used in a statistical function. >
!+DTR_I18N V6.1
!       Expanded message for I18N.
.BASE 851
BADCSNAM/FAO=4/WARNING-
<Unspported character set was specified.  Default value will be used.>
!-DTR_I18N V6.1
.END
                                                                                                                                                                                                                                                                                                                                                                                  �               * [C]DTRMSGS_JA.MSG;1 +  , �   . �    / s P   4 �   �   � G                    - 
    0  � 1    2   3      K � P   W   O �    5   6 `��j�*�  7 `��)�*�  8          9          G s P  H �� J �              �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          m��� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         � "            .TITLE DTRMSGS VAX Datatrieve error messages
.FACILITY	DTR, 141 /SYSTEM /PREFIX=DTR$_
.IDENT 'VAX DTR V6.1-0'
.BASE 1
BADBLK/FAO=2/SEVERE-
<�������顼(ͽ�������֥��å� ID �� !UB �Ǥ���Τˡ�!UB �˽в񤤤ޤ�����)>
!<Internal error (expected block id !UB, encountered id !UB). >
.BASE 2
BADSUBBLK/FAO=2/SEVERE-
<�������顼(ͽ�������Ρ��ɡ������פ� !UB �Ǥ���Τˡ�!UB �˽в񤤤ޤ�����)>
!<Internal error (expected node type !UB, encountered type !UB). >
.BASE 3
BADVARTYP/FAO=0/SEVERE-
<�������顼�� SYS$DEC �֥��å����������ѿ������פ�����ޤ�������>
!<Internal error (bad variable type in SYN$DEC block). >
.BASE 4
BLKTOOBIG/FAO=0/ERROR-
<�֥��å�����Ĺ��ۤ��ޤ�����ʸ�Ͻ�������ޤ���>
!<Maximum block size exceeded; statement aborted. >
.BASE 5
INVBLKTYP/FAO=0/SEVERE-
<�������顼�������ʥ֥��å��������פ�Ϳ�����ޤ�������>
!<Internal error (invalid block type given). >
.BASE 6
INVDSCTYP/FAO=0/SEVERE-
<�Ѵ��Τ���ε��һҥ����פ�����������ޤ���>
!<Invalid descriptor type for conversion. >
.BASE 7
INVPOOLCL/FAO=0/SEVERE-
<�������顼�ʻ��ꤵ�줿�ס��롦���饹������������ޤ��󡣡�>
!<Internal error (invalid pool class given). >
.BASE 8
RELBADBLK/FAO=0/SEVERE-
<�������顼�����ɥ֥��å���������褦�Ȥ��ޤ�������>
!<Internal error (attempted to release bad block). >
.BASE 9
RELBADPOL/FAO=0/SEVERE-
<�������顼�������ʥס����������褦�Ȥ��ޤ�������>
!<Internal error (attempted to release bad pool). >
.BASE 10
NOTYETIMP/FAO=0/SEVERE-
<�������顼�ʵ�ǽ���ޤ�����ץ���Ȥ���Ƥ��ޤ��󡣡�>
!<Internal error (feature is not implemented yet). >
.BASE 11
NOCURTOK/FAO=0/SEVERE-
<�������顼�ʸ��ߤΥȡ����󡦥֥��å�������ޤ��󡣡�>
!<Internal error (no current token block). >
.BASE 12
SYSBADARG/FAO=0/SEVERE-
<�������顼�ʰ����θĿ�������������ޤ��󡣡�>
!<Internal error (invalid number of arguments). >
.BASE 13
CASOUTRAN/FAO=0/SEVERE-
<�������顼�ʥ��եȥ���������������å�����>
!<Internal error (software consistency check). >
.BASE 14
NULROC/FAO=0/SEVERE-
<�������顼��null record occurence block��.>
!<Internal error (null record occurrence block). >
.BASE 15
DOMLOST/FAO=0/SEVERE-
<�������顼�ʥ����ƥࡦ�ꥹ�Ȥ���ɥᥤ�󤬼����ޤ�������>
!<Internal error (domain lost from system list). >
.BASE 16
HSHBLKLOS/FAO=0/SEVERE-
<�������顼�ʥϥå��塦�֥��å��������ޤ�������>
!<Internal error (hash block lost). >
.BASE 17
HOPELESS/FAO=0/SEVERE-
<�������顼�ʥ��եȥ���������������å�����>
!<Internal error (software consistency check). >
.BASE 18
FSGNORID/FAO=0/SEVERE-
<�������顼�ʥե����롦�������Ȥ˥쥳���� ID ������ޤ��󡣡�>
!<Internal error (file segment missing record ID). >
.BASE 19
GETRIDFAIL/FAO=0/SEVERE-
<�������顼��RID �ˤ��쥳���ɤμ��Ф����Ǥ��ޤ���Ǥ�������>
!<Internal error (failed to get record by RID). >
.BASE 20
NOKEEPNEE/FAO=0/SEVERE-
<�������顼�ʥ����ץꥹ�Ȥ�ɬ�פʤΤˤ��줬����ޤ��󡣡�>
!<Internal error (no keeplist when one is needed). >
.BASE 21
KPLNOSSC/FAO=0/SEVERE-
<�������顼�ʥ��֥������ޤ����Ѥ���Ƥ��ʤ��Τ˥����ץꥹ�Ȥ��ꥯ�����Ȥ���ޤ�������>
!<Internal error (keeplist requested when no subschema is in use). >
.BASE 22
NULPRIELT/FAO=0/SEVERE-
<�������顼�ʥ���ƥ����ȡ��������㡼����Ǥϰ������٤����Ǥ�����ޤ��󡣡�>
!<Internal error (null print element in context searcher). >
.BASE 23
ERROR/FAO=0/ERROR-
<���顼�Τ����ʸ����������ޤ�����>
!<Statement abandoned due to error. >
.BASE 24
NO_DFPID/FAO=0/ERROR-
<PW, UIC, USER, TERMINAL �Τ����줫����ꤷ�ʤ���Фʤ�ޤ���>
!<One of PW, UIC, USER, or TERMINAL must be specified. >
.BASE 25
NO_DFPPRI/FAO=0/ERROR-
<GRANT, DENY, BANISH �Τ����줫����ꤷ�ʤ���Фʤ�ޤ���>
!<One of GRANT, DENY, or BANISH must be specified. >
.BASE 26
BADDFPTER/FAO=0/ERROR-
<DEFINEP ���ޥ�ɤ� TERMINAL ���꤬����������ޤ���>
!<Invalid TERMINAL specification for DEFINEP command. >
.BASE 27
BADUIC/FAO=0/ERROR-
<���ꤵ�줿 UIC ������������ޤ���>
!<Invalid UIC specified. >
.BASE 28
PRIVVIOL/FAO=0/ERROR-
<�ø���ȿ -- �������������ݤ���ޤ�����>
!<Privilege violation -- access denied. >
.BASE 29
NOPOOLIDS/FAO=0/SEVERE-
<�������顼�ʻ��Ѳ�ǽ��ưŪ���ꡦ�ס��뤬�����Ȥ��Ԥ���Ƥ��ޤ�����>
!<Internal error (all available dynamic memory pools have been exhausted). >
.BASE 30
BADHANDLE/FAO=0/ERROR-
<̵�� DDMF �ϥ�ɥ롣>
!<Invalid DDMF handle. >
.BASE 31
BADNUMARG/FAO=0/ERROR-
<DDMF ���Ф�������θĿ�������������ޤ���>
!<Invalid number of arguments to DDMF. >
.BASE 32
USESLOEXH/FAO=0/ERROR-
<DDMF �桼���������åȤ��Ȥ��ڤ��Ƥ��ޤ���!/�̤� DATATRIEVE ���ȥ꡼��������Ǥ��ޤ���>
!<DDMF user slots are exhausted - cannot initialize another DATATRIEVE stream. >
.BASE 33
WRONGSTALL/FAO=0/ERROR-
<���ߤΥ��ȡ���ݥ���Ȥ��Ф���ƽФ������äƤ��ޤ���>
!<Wrong call for current stallpoint. >
.BASE 34
SYNTAX/FAO=3/ERROR-            
<!AC ��ͽ�������Τˡ�"!AD" �����Ϥ���ޤ�����>
!<Expected !AC, encountered "!AD". >
.BASE 35
DUPCLASPE/FAO=2/ERROR-
<��ʣ�᤬���ꤵ��ޤ�����"!AD" >
!<Duplicate clause specified: "!AD". >
.BASE 36
FLDNOTRED/FAO=2/ERROR-
<REDEFINES �Υե������ "!AD" �����Ĥ���ޤ���>
!<Field "!AD" not found for REDEFINES. >
.BASE 37
FLDNOTOCC/FAO=2/ERROR-                 
<OCCURS DEPENDING ON �� �ե������ "!AD" �����Ĥ���ޤ���>
!<Field "!AD" not found for OCCURS DEPENDING ON. >
.BASE 38
MULOCCDEP/FAO=0/ERROR-
<ʣ���� OCCURS DEPENDING ON ��ϵ�����ޤ���>
!<Multiple OCCURS DEPENDING ON clauses are not allowed. >
.BASE 39
OCCNOTLAS/FAO=0/ERROR-
<OCCURS DEPENDING ON �Τ��Ȥˤϥ�٥��ֹ�Τ��⤤�ե�����ɤ�����������ޤ���>
!<Only fields with higher level numbers are allowed after an OCCURS DEPENDING ON. >
.BASE 40
OCCOUTRAN/FAO=0/ERROR-
<OCCURS DEPENDING ON ������Ȥ��ϰϳ��Ǥ���>
!<OCCURS DEPENDING ON count out of range. >
.BASE 41
ILLPICSTR/FAO=2/ERROR-
<PICTURE �ޤ��� EDIT ʸ�����ʸ����"!AD"������������ޤ���>
!<Illegal PICTURE or EDIT string character: "!AD". >
.BASE 42
PICUSAMAT/FAO=0/ERROR-
<���ιԤ� PICTURE �� USAGE �᤬�б����ޤ���>
!<PICTURE and USAGE clauses don't match in previous line. >
.BASE 43
TOOMANDIG/FAO=0/ERROR-
<������η�����礭�����ޤ��ʺ���ϣ����ˡ�>
!<Too many digits in numeric string (max is 18). >
.BASE 44
MISPICUSA/FAO=0/ERROR-
<���ιԤ� PICTURE �ޤ��� USAGE �᤬����ޤ���>
!<Missing PICTURE or USAGE clause in previous line. >
.BASE 45
BADLENRED/FAO=2/ERROR-
<REDEFINES �Υե������ "!AD" ��Ĺ��������������ޤ���>
!<Bad length for REDEFINES, field "!AD". >
.BASE 46
LINTOOLON/FAO=0/ERROR-
<���ޥ�ɹԤ�������ʸ����Ķ���Ƥ��ޤ���>
!<Command line exceeds 255 characters. >
.BASE 47
UNTERQUO/FAO=0/ERROR-
<������ǰϤޤ줿ʸ����̤�����Ǥ���>
!<Unterminated quoted string. >
.BASE 48
MISTERMIN/FAO=0/ERROR-
<�Ԥν�λ�򼨤����ߥ����󤬤���ޤ���>
!<Missing line terminator (semicolon). >
.BASE 49
DICELTUSE/FAO=1/ERROR-
<�������� "!AS" �Ϥ��Ǥ˻��Ѥ���Ƥ��ޤ���>
!<Dictionary element "!AS" already in use. >
.BASE 50
ELTNOTDIC/FAO=1/ERROR-
<���� "!AS" �ϼ���˸��Ĥ���ޤ���>
!<Element "!AS" not found in dictionary. >
.BASE 51
ELTNOTDTR/FAO=1/ERROR-
<���� "!AS" �ϥɥᥤ��, �쥳����, �ץ�������, �ơ��֥�Τ�����Ǥ⤢��ޤ���>
!<Element "!AS" is not a Domain, Record, Procedure, or Table. >
.BASE 52
DICNOTUND/FAO=0/ERROR-
<�������Ǥ����Ǥ��ޤ���>
!<Dictionary element not understood. >
.BASE 53
NOTRDYDOM/FAO=2/ERROR-
<"!AD" �Ϥޤ� 'READY' ����Ƥ��ޤ���>
!<"!AD" is not a readied source. >
.BASE 54
DOMNOTRDY/FAO=1/ERROR-
<�ɥᥤ�� "!AC" �Ϥޤ� 'READY' ����Ƥ��ޤ���>
!<Domain "!AC" not ready. >
.BASE 55
NOTRECNAM/FAO=1/ERROR-
<"!AS" �ϥ쥳����̾�ǤϤ���ޤ���>
!<"!AS" is not a record name. >
.BASE 56
NOTDOMNAM/FAO=1/ERROR-
<"!AS" �ϥɥᥤ��̾�ǤϤ���ޤ���>
!<"!AS" is not a domain name. >
.BASE 57
NOTPRCNAM/FAO=1/ERROR-
<"!AS" �ϥץ�������̾�ǤϤ���ޤ���>
!<"!AS" is not a procedure name. >
.BASE 58
NOTTABNAM/FAO=1/ERROR-
<"!AS" �ϥơ��֥�̾�ǤϤ���ޤ���>
!<"!AS" is not a table name. >
.BASE 59
NOTPLTNAM/FAO=1/ERROR-
<"!AS" �ϥץ��å�̾�ǤϤ���ޤ���>
!<"!AS" is not a plot name. >
.BASE 60
NOTDOMAIN/FAO=2/ERROR-
<"!AD" �ϥ�ǥ����ɥᥤ��, ���쥯�����, �ꥹ�ȤΤ�����Ǥ⤢��ޤ���>
!<"!AD" is not a readied source, collection, or list. >
.BASE 61
NOTLIST/FAO=2/SEVERE-
<"!AD" �ϥꥹ�ȤǤϤ���ޤ���>
!<"!AD" is not a list. >
.BASE 62
NOCONTEXT/FAO=2/ERROR-
<"!AD" ��̤��������뤤��ʸ̮���ǻ��Ѥ���Ƥ��ޤ���>
!<"!AD" is undefined or used out of context. >
.BASE 63
NULPRILIS/FAO=0/ERROR-
<�������٤���Τ�����ޤ���>
!<Null print list. >
.BASE 64
BADNUMBER/FAO=2/ERROR-
<̵���ֹ桧 "!AD"��>
!<Bad number: "!AD". >
.BASE 65
MOV_RANGE/FAO=0/ERROR-
<Item movement beyond allowed range. >
!<***** 65 ̤����>
.BASE 66
UNSCONV/FAO=0/ERROR-
<�ǡ����Ѵ������ݡ��Ȥ���Ƥ��ޤ���>
!<Unsupported data conversion. >
.BASE 67
UNWIND/�                                                                                                                                                                                                                                                                           ��Zw $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         � "            FAO=0/ERROR-
<���ڥ졼���ˤ��¹Ԥ���λ����ޤ�����>
!<Execution terminated by operator. >
.BASE 68
COMPOVF/FAO=0/ERROR-
<�׻��Υ����Хե����Ǥ���>
!<Computation overflow. >
.BASE 69
UNXDICTYP/FAO=0/ERROR-
<�����ͽ�����ʤ�°�������פ˽в񤤤ޤ�����>
!<Unexpected attribute type encountered in dictionary. >
.BASE 70
REQDICMIS/FAO=1/ERROR-
<ɬ�פʼ���°�� !AC ��������Ƥ��ޤ���>
!<Required dictionary attribute !AC missing. >
.BASE 71
WRIACCREQ/FAO=1/ERROR-
<"!AC" �ˤϽ���ߥ���������ɬ�פǤ���>
!<Write access is required for "!AC". >
.BASE 72
MODACCREQ/FAO=1/ERROR-
<"!AC" �ˤϽ�������������ɬ�פǤ���>
!<Modify access is required for  "!AC". >
.BASE 73
PATNOTDIR/FAO=1/ERROR-
<�ѥ����Ρ��� "!AS" �ϥǥ��쥯�ȥ�ǤϤ���ޤ���>
!<Path node "!AS" is not a directory. >
.BASE 74
DELDIRNOD/FAO=0/ERROR-
<�ǥ��쥯�ȥꡦ�Ρ��ɤ�����Ǥ��ޤ���>
!<Can't delete a directory node. >
.BASE 75
BADRECSIZ/FAO=2/WARNING-
<̵���쥳���ɡ����������������!UW�� �ե����롧!UW >
!<Bad record size. Defined: !UW  File: !UW >
.BASE 76
PRISTALOS/FAO=0/ERROR-
<�����Υ��ޥ�ɤ������ޤ�����>
!<Prior command lost.  Sorry. >
.BASE 77
TXTNOTFND/FAO=1/ERROR-
<!AS�Υ��������ƥ����Ȥ�����˸��Ĥ���ޤ���>
!<Source text for !AS not found in dictionary. >
.BASE 78
BADROCKBF/FAO=0/SEVERE-
<�������顼�� ROC�����������ϥ������Хåե�������������ޤ��󡣡�>
!<Internal error (bad ROC chain or key buffer). >
.BASE 79
BADSORREC/FAO=0/SEVERE-
<�������顼�ʥ����ȡ��쥳����Ĺ������������ޤ��󡣡�>
!<Internal error (bad sort record length). >
.BASE 80
MISSUBPAR/FAO=0/ERROR-
<���ޥ��ʸ������ִ����ѥ�᡼��������ޤ���>
!<Missing substitution parameter in command string. >
.BASE 81
BADSUBPAR/FAO=0/ERROR-
<���ޥ��ʸ������ִ����ѥ�᡼��������������ޤ���>
!<Bad substitution parameter in command string. >
.BASE 82
BADSUBDES/FAO=0/ERROR-
<Bad substitution descriptor argument. >
!<***** 82 ̤����>
.BASE 83
EDTNOTINS/FAO=0/ERROR-
< EDT�����󥹥ȡ��뤵��Ƥ��ޤ���TECO���Ʋ�������>
!<EDT is not installed.  Try TECO. >
.BASE 84
RERDYFAIL/FAO=0/ERROR-
<READY ���ޤ����Ԥ��ޤ������ɥᥤ��ϼ�ưŪ�˽�λ���ޤ�����>
!<Re-ready failed. Domain automatically finished. >
.BASE 85
NOCOMP6/FAO=0/ERROR-
<COMP-6 �ǡ������ϥ���ץ���Ȥ���Ƥ��ޤ���>
!<COMP-6 data type is not implemented. >
.BASE 86
RDSNOTACT/FAO=0/ERROR-
<��֥����� (DDMF) ��ư�Ǥ��ޤ���Ǥ�����>
!<Remote Server (DDMF) could not be activated. >
.BASE 87
RDSTERMIN/FAO=0/ERROR-
<��֥����� (DDMF) ��������λ���ޤ�����>
!<Remote Server (DDMF) terminated unexpectedly. >
.BASE 88
DBMINCSSC/FAO=0/ERROR-
<����� DBMS���֥������ޤ�̷�⤷�Ƥ��ޤ���>
!<Inconsistent DBMS subschema in dictionary. >
.BASE 89
DBMSSCDOM/FAO=0/ERROR-
<���֥������ޤ����Ѥ���Ƥ��ޤ� �� �ɥᥤ��� 'READY' �Ǥ��ޤ���>
!<One subschema already in use - can't ready domain. >
.BASE 90
INFBADID/FAO=0/ERROR-
<���֥�������ID��̵���Ǥ���>
!<Bad object id. >
.BASE 91
INFBADCOD/FAO=0/ERROR-
<���󥳡��ɤ�̵���Ǥ���>
!<Bad information code. >
.BASE 92
INFBADTYP/FAO=0/ERROR-
<��������뤿��Υǡ����������פ�����������ޤ���>
!<Bad data type to receive information. >
.BASE 93
INFNOTFOU/FAO=0/ERROR-
<���֥������Ȥ����Ĥ���ޤ���>
!<Object not found. >
.BASE 94
REMSYNERR/FAO=0/ERROR-
<DDMF ���Ʊ�������顼�Ǥ���>
!<DDMF remote synchronization error. >
.BASE 95
ABORT/FAO=1/ERROR-
<ABORT: !AS >
!<ABORT��!AS >
.BASE 96
NOTSET/FAO=2/ERROR-
<"!AD" �ϥ��åȤǤϤ���ޤ���>
!<"!AD" is not a set. >
.BASE 97
MISSETCTX/FAO=2/ERROR-
<���å� "!AD" ��̤��������뤤��ʸ̮���ǻ��Ѥ���Ƥ��ޤ���>
!<Set "!AD" is undefined or used out of context. >
.BASE 98
PAGEINUSE/FAO=0/SEVERE-
<�������顼�ʻ�����Υڡ�����������褦�Ȥ��ޤ�������>
!<Internal error (attempt to release a page in use). >
.BASE 99
VBNOUTRAN/FAO=0/SEVERE-
<�������顼�ʥե�����β��ۥ֥��å��ֹ椬�ϰϳ��Ǥ�����>
!<Internal error (file's virtual block number out of range). >
.BASE 100
PAGOUTRAN/FAO=0/SEVERE-
<�������顼�ʥե����롦�������ȤΥڡ����ֹ椬�ϰϳ��Ǥ�����>
!<Internal error (page number out of range for file segment). >
.BASE 101
NOFREEBCB/FAO=0/SEVERE-
<�������顼�ʥХåե�����֥��å��������Ƥ��ޤ��󡣡�>
!<Internal error (no free buffer control blocks). >
.BASE 102
NOTINMEM/FAO=0/SEVERE-
<�������顼�ʥڡ���������ˤ���ޤ��� - �����ޤ��ϥޡ������Ǥ��ޤ��󡣡�>
!<Internal error (page is not in memory - can't release or mark). >
.BASE 103
PAGLSTUSR/FAO=0/SEVERE-
<�������顼�ʥڡ������Хåե��λ��ѼԤ������Ǥ�����>
!<Internal error (user of a page buffer got lost). >
.BASE 104
NOCURCOL/FAO=0/ERROR-
<���ߤΥ��쥯��������ꤵ��Ƥ��ޤ���>
!<A current collection has not been established. >
.BASE 105
NOCOLSEL/FAO=0/ERROR-
<���쥯����󤬤ʤ��Τ� SELECT �Ǥ��ޤ���>
!<No collection for select. >
.BASE 106
NOCOLSOR/FAO=0/ERROR-
<���쥯����󤬤ʤ��Τ� SORT �Ǥ��ޤ���>
!<No collection for sort. >
.BASE 107
OUTRANCOL/FAO=0/ERROR-
<���쥯�����Υ쥳�����ֹ椬�ϰϳ��Ǥ���>
!<Record number out of range for collection. >
.BASE 108
SELNOTFND/FAO=0/ERROR-
<���򤵤줿�쥳���ɤ����Ĥ���ޤ���>
!<Selected record not found. >
.BASE 109
RECTOOBIG/FAO=0/ERROR-
<�쥳���ɤ��礭�����ƽ缡���ѹ��Ǥ��ޤ���>
!<Record too big for sequential update. >
.BASE 110
WRODOMTYP/FAO=2/ERROR-
<"!AS" �� RMS �ɥᥤ��ǤϤ���ޤ���>
!<"!AS" is not an RMS domain. >
.BASE 111
KEYUNDEF/FAO=4/ERROR-
<���� "!AF" �ϥɥᥤ�� "!AF" ���������Ƥ��ޤ���>
!<Key "!AF" is not defined in domain "!AF". >
.BASE 112
OBJTOOBIG/FAO=0/ERROR-
<�������֥������Ȥ��礭�����ƹԤ����������ڤ�ޤ���>
!<Print object is too large for line width. >
.BASE 113
VALIDERR/FAO=2/ERROR-
<�ե������ !AD ���������������顼�Ǥ���>
!<Validation error for field !AD. >
.BASE 114
ASSVIRFLD/FAO=0/ERROR-
<���ۥե�����ɤ˳�����Ƥ뤳�ȤϤǤ��ޤ���>
!<Cannot assign to a virtual field. >
.BASE 115
DIGNUMPIC/FAO=0/ERROR-
<�����ԥ�����ޤ����Խ�ʸ����λ�����¿�����ޤ���>
!<Too many digits in numeric picture or edit string. >
.BASE 116
ZEROOBJ/FAO=0/INFORMATIONAL-
<���������֥������Ȥ� MAX, MIN, �ޤ��� AVERAGE ��Ȥ뤳�ȤϤǤ��ޤ���>
!<Can't take MAX, MIN, or AVERAGE of zero objects. >
.BASE 117
NOTINTAB/FAO=0/WARNING-
<�ơ��֥���ͤ����Ĥ���ޤ���>
!<Value not found in table. >
.BASE 118
NOVALFROM/FAO=0/ERROR-
<�쥳���ɤޤ��ϥơ��֥���ͤ����Ĥ���ޤ���>
!<Value not found from record or table. >
.BASE 119
NOTRGREC/FAO=1/ERROR-
<!AC �Υ������åȡ��쥳���ɤ�����ޤ���>
!<No target record for !AC. >
.BASE 120
CNVNUMDAT/FAO=0/ERROR-
<��������ʸ������Ѵ��Ǥ��ޤ���>
!<Can't convert number to a date string. >
.BASE 121
RECERSD/FAO=0/ERROR-
<������줿�쥳���ɤΥե�����ɤ���Ѥ��뤳�ȤϤǤ��ޤ���>
!<You cannot use a field from a record that has been erased. >
.BASE 122
NODBMERAS/FAO=0/ERROR-
<DBMS�쥳���ɤϺ���Ǥ��ޤ���>
!<Can't erase DBMS records - sorry. >
.BASE 123
SELRECREQ/FAO=0/ERROR-
<���򤵤줿�쥳���ɤϥ��쥯�����ˤ���ޤ���>
!<No selected record in collection. >
.BASE 124
RECNOTDBM/FAO=2/ERROR-
<���쥯����� !AD �ˤ���쥳���ɤ� DBMS�쥳���ɤǤϤ���ޤ���>
!<Record in collection !AD is not DBMS record. >
.BASE 125
NOTMEMSET/FAO=3/ERROR-
<�ɥᥤ�� !AC �ϥ��å� !AD �Υ��С��ǤϤ���ޤ���>
!<Domain !AC is not a member of set !AD. >
.BASE 126
NOTOWNSET/FAO=3/ERROR-
<�ɥᥤ�� !AC �ϥ��å� !AD �ν�ͭ�ԤǤϤ���ޤ���>
!<Domain !AC is not an owner of set !AD. >
.BASE 127
IRRELESET/FAO=3/ERROR-
<�ɥᥤ�� !AC �ϥ��å� !AD �˴ط��Ť����Ƥ��ޤ���>
!<Domain !AC is not related to set !AD. >
.BASE 128
NODBMDOM/FAO=0/INFORMATIONAL-
<DBMS�ɥᥤ��� 'READY' ����Ƥ��ޤ���COMMIT ���� ROLLBACK ʸ��̵���Ǥ���>
!<No DBMS domains readied - nothing to COMMIT or ROLLBACK. >
.BASE 129
BADINSERT/FAO=2/ERROR-
<���å� !AD �Τ���������ϼ¹Ԥ��Բ�ǽ�ˤ��ޤ���>
!<Insertion for set !AD precludes execution. >
.BASE 130
MISAUTSET/FAO=2/ERROR-
<��Ǽ���뤿��ˤ� AUTOMATIC ���å� !AD ����ꤷ�ʤ���Фʤ�ޤ���>
!<AUTOMATIC set !AD must be specified for store. >
.BASE 131
RPOEXCPRI/FAO=0/ERROR-
<REPORT �� PRINT ʸ��¿�����ޤ���>
!<Excessive PRINT statements for REPORT. >
.BASE 132
RPOFMTRJT/FAO=0/ERROR-
<REPORT �ե����ޥåƥ����᤬ʸ̮�ˤ��äƤ��ޤ���>
!<REPORT formatting clause out of context.> 
.BASE 133
MISNOTFLD/FAO=0/ERROR-
<MISSING ���ڥ졼���Υ��֥������Ȥϥե�����ɤǤʤ���Фʤ�ޤ���>
!<Object of MISSING operator must be a field. >
.BASE 134
PLONOTFOU/FAO=0/ERROR-
<�ץ��å�������ߤĤ���ޤ���>
!<Plot definition not found. >
.BASE 135
PLOENTMIS/FAO=0/ERROR-
<�ץ��åȤΥ���ȥꡦ�ݥ���Ȥ�̤����Ǥ���>
!<Plot entry point is not defined. >
.BASE 136
PLOBADARG/FAO=0/ERROR-
<�ץ��å�����Υѥ�᡼����¿�����ޤ���>
!<Too many parameters for plot definition. >
.BASE 137
RPODUPAT/FAO=2/ERROR-
<!AD �˻��ꤵ�줿����֥��å�����ʣ���Ƥ��ޤ���>
!<Duplicate control�                                                                                                                                                                                                                                                                           6��� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         Ό "     #        break specified for !AD. >
.BASE 138
RPODUPSET/FAO=1/ERROR-
<SET ���ץ���� !AC ����ʣ���ƻ��ꤵ��ޤ�����>
!<Duplicate SET option !AC specified. >
.BASE 139
PLOIDXRAN/FAO=3/ERROR-
<�ץ��åȡ�����ǥå������ϰϳ��Ǥ����٥�����!AD, ������ǥå�����!SL >
!<Plot index out of range.  Vector: !AD, index: !SL >
.BASE 140
PLOLOCRAN/FAO=2/ERROR-
<�ץ��åȰ��� FN ���ϰϳ��Ǥ��� FN: !AC����: !SL >
!<Plot location fn out of range.  Fn: !AC, value: !SL >
.BASE 141
NOTVALPRI/FAO=2/ERROR-
<"!AD" ��ͭ�����ø������ɤǤϤ���ޤ���>
!<"!AD" is not a valid privilege code. >
.BASE 142
CLANOTPER/FAO=1/ERROR-
<�ѿ�������� !AC ��ϵ�����ޤ���>
!<!AC clause is not permitted on a variable declaration. >
.BASE 143
NOCOLDROP/FAO=0/ERROR-
<����쥳���ɤ�ޤ�Ǥ��륳�쥯����󤬤ʤ��Τ� DROP �Ǥ��ޤ���>
!<No collection with selected record for DROP. >
.BASE 144
RECPREDRO/FAO=0/ERROR-
<���쥯����󤫤�쥳���ɤ� DROP ����Ƥ��ޤ���>
!<Record has been dropped from the collection. >
.BASE 145
VIEFIEUND/FAO=2/ERROR-
<�ե������ !AS �ϥɥᥤ�� !AS �˸��Ĥ���ޤ���>
!<Field !AS cannot be found in domain !AS. >
.BASE 146
VIEDOMUND/FAO=1/ERROR-
<�ɥᥤ�� !AS ���ɥᥤ�󡦥ꥹ����ˤʤ��Τ� VIEW �ν���������ޤ���>
!<Domain !AS was not included in domain list for view. >
.BASE 147
WARNING/FAO=0/WARNING-
<���ơ��ȥ��Ȥν����������ޤ���������˥󥰤�����ޤ���>
!<Statement completed with warning. >
.BASE 148
DATCNVERR/FAO=1/WARNING-
<����ʸ���� "!AS" ����Ѵ����顼�Ǥ���>
!<Conversion error in date string "!AS". >
.BASE 149
COMPLEX/FAO=0/WARNING-
<ʣ�ǥǡ����������פΥǡ����Ѵ����顼�Ǥ���>
!<Data conversion error on complex data type. >
.BASE 150
UNDERFLOW/FAO=0/WARNING-
<�ǡ����Ѵ�������ե����Ǥ���>
!<Data conversion underflow. >
.BASE 151
NONDIGIT/FAO=1/WARNING-
<ʸ���� "!AS" ����������ޤޤ�Ƥ��ޤ�������ʸ����̵�뤷�ޤ���>
!<Non-digit in string "!AS", ignoring character(s). >
.BASE 152
CONSIGUSA/FAO=1/WARNING-
<SIGN ��� USAGE �᤬̷�⤷�Ƥ��ޤ���"!AC" ��̵�뤷�ޤ���>
!<Conflicting SIGN and USAGE clauses, ignoring "!AC". >
.BASE 153
CONVALCOM/FAO=0/WARNING-
<VALID ��� COMPUTED �᤬̷�⤷�Ƥ��ޤ���>
!<Conflicting VALID and COMPUTED clauses. >
.BASE 154
OVERFLOW/FAO=0/WARNING-
<�ǡ����Ѵ������Хե���>
!<Data conversion overflow. >
.BASE 155
NOACCCHG/FAO=0/WARNING-
<��ǥ����ɥᥤ����Ф��륢���������ѹ��Ǥ��ޤ���Ǥ�����>
!<Couldn't change access to readied domain. >
.BASE 156
ASSUMELIT/FAO=2/WARNING-
<"!AD" �ϥե�����ɤǤϤʤ�����ƥ��Ȥߤʤ���ޤ���>
!<"!AD" not field, assumed literal. >
.BASE 157
DICELECOR/FAO=2/WARNING-
<�������� "!AD" �������ǡ����������Ǥ��ޤ���>
!<Dictionary element "!AD" is corrupt and cannot be accessed. >
.BASE 158
DIRCMPSTK/FAO=1/SEVERE-
<�������顼�ʥ���ѥ��顦�����å��Υ��顼����٥� !UL�ˡ�>
!<Internal error (dirty compiler stack, level !UL). >
.BASE 159
DIVBYZERO/FAO=0/WARNING-
<�����ˤ���껻��Ԥ����Ȥ��ޤ������ͤȤ��� -1 ���֤��ޤ���>
!<Attempt to divide by zero. Returning -1 as a value. >
.BASE 160
TRUDURASS/FAO=0/WARNING-
<�����������ڤ�ΤƤ�����ޤ�����>
!<Truncation during assignment. >
.BASE 161
OVFDURMUL/FAO=0/WARNING-
<�ݤ����δ֤˥����Хե���������ޤ�����>
!<Overflow during multiplication. >
.BASE 162
COMPURCOL/FAO=2/WARNING-
<COMMIT �ޤ��� ROLLBACK ���¹Ԥ���ޤ�����!/���쥯����� !AD ����ưŪ�˲�������ޤ�����>
!<COMMIT or ROLLBACK executed; collection !AD automatically released. >
.BASE 163
STAMISDAT/FAO=2/INFORMATIONAL-
<�ؿ��� !UL �� (���� !UL ��) ���ͤ��Ѥ��Ʒ׻�����ޤ�����>
!<[Function computed using !UL of !UL values.] >
.BASE 164
MISCLAMIS/FAO=2/WARNING-
<!AD ���Ф��� MISSING VALUE ��̤����ʤΤǡ���ά�����ͤ���Ѥ��ޤ���>
!<MISSING VALUE not defined for !AD, using default value. >
.BASE 165
NORECSEL/FAO=0/WARNING-
<�쥳���ɤ����򤵤�Ƥ��ʤ��Τǡ������쥯������������ޤ���>
!<No record selected, printing whole collection. >
.BASE 166
EXPCOTAVA/FAO=1/WARNING-
<���쥯����󡦥ơ��֥�,�ޤ����ѿ�������٤��ʤΤˡ�"!AS" �˽в񤤤ޤ�����>
!<Expected collection, table, or variable, encountered "!AS". >
.BASE 167
CTXMAGIC/FAO=0/WARNING-
<��ʬ��ʸ̮�ǤϤ���ޤ���!/����ƥ����ȡ��������㡼�ˤ�ꤤ���Ĥ��Υե������̾����褵��ޤ�����>
!<Not enough context. Some field names resolved by Context Searcher. >
.BASE 168
PROMPT/FAO=1/INFORMATIONAL-
<����  !AS: >
!<Enter !AS: >
.BASE 169
RE_PROMPT/FAO=1/INFORMATIONAL-
<������  !AS: >
!<Re-enter !AS: >
.BASE 170
PW_PROMPT/FAO=1/INFORMATIONAL-
<�ѥ���ɤ�����  !AS: >
!<Enter password for !AS: >
.BASE 171
SIGNON/FAO=4/INFORMATIONAL-
<���ܸ� DEC DATATRIEVE !AD!UL.!UL!/���ܣģţø�����𥷥��ƥ�!/���ä����� HELP �ȥ����פ��Ʋ�������!/>  
!<DEC DATATRIEVE !AD!UL.!UL!/Digital Query and Report System!/Type HELP for help!/>
.BASE 172       
SUCCESS/FAO=0/INFORMATIONAL-
<ʸ������˽�λ���ޤ�����>
!<Statement completed successfully. >
.BASE 173
SHUTDOWN/FAO=0/INFORMATIONAL-
<DATATRIEVE/DDMF ����ߤ��ޤ���>
!<Shut down DATATRIEVE/DDMF. >
.BASE 175
LOOKINFOR/FAO=1/INFORMATIONAL-
<[!AC ���ܤ��Ƥ��ޤ�]>
!<[Looking for !AC] >
.BASE 176
FLUSH/FAO=0/INFORMATIONAL-
<���󥿥å��������顼�����Ϥ��˴����ޤ���>
!<Syntax error, flushing input. >
.BASE 177
RECLENGTH/FAO=1/INFORMATIONAL-
<[�쥳���ɤ� !UL �Х���Ĺ�Ǥ���]>
!<[Record is !UL byte!%S long.] >
.BASE 178
DICTIONARY/FAO=1/INFORMATIONAL-
<��ά���μ���� !AC �Ǥ���>
!<The default directory is !AC >
.BASE 179
DOMAINS/FAO=0/INFORMATIONAL-
<�ɥᥤ��:>
!<Domains: >
.BASE 180
NODOMGLO/FAO=0/INFORMATIONAL-
<��ǥ����ɥᥤ��ޤ��ϥ������Х��ѿ����������Ƥ��ޤ���>
!<No ready sources or global variables declared. >
.BASE 181
PROCEDURES/FAO=0/INFORMATIONAL-
<�ץ���������: >
!<Procedures: >
.BASE 182
PLOTS/FAO=0/INFORMATIONAL-
<�ץ��å�:>
!<Plots: >
.BASE 183
NOREADOM/FAO=0/INFORMATIONAL-
<��ǥ����ɥᥤ��Ϥ���ޤ���>
!<No ready sources. >
.BASE 184
READYDOM/FAO=0/INFORMATIONAL-
<��ǥ����ɥᥤ��: >
!<Ready domains: >
.BASE 185
RECORDS/FAO=0/INFORMATIONAL-
<�쥳����:>
!<Records: >   
.BASE 186
TABLES/FAO=0/INFORMATIONAL-
<�ơ��֥�:>
!<Tables: >
.BASE 187
DICTIONARIES/FAO=0/INFORMATIONAL-
<����:>
!<Dictionaries: >
.BASE 188
DELNOTFOU/FAO=0/INFORMATIONAL-
<���񥪥֥������Ȥ�����ޤ���>
!<Dictionary object does not exist. >
.BASE 189
EDTINPUT/FAO=0/INFORMATIONAL-
<EDT �����Ϥ��Ʋ�������>
!<EDT wants input. >
.BASE 190
EDTOUTPUT/FAO=1/INFORMATIONAL-
<EDT ����ν��ϡ�!AS.>
!<EDT has output: !AS. >
.BASE 191
EDTEXIT/FAO=0/INFORMATIONAL-
<EDT ����λ���ޤ�����>
!<EDT is exiting. >
.BASE 192
REMOTEMSG/FAO=2/INFORMATIONAL-
<[DDMF] !AD. >
!<[DDMF] !AD.>
.BASE 193
DISPLAY/FAO=1/INFORMATIONAL-
<�ǥ����ץ쥤: !AS >
!<DISPLAY: !AS >
.BASE 194
RECFOUND/FAO=1/INFORMATIONAL-
<[!UL ��Υ쥳���ɤ����Ĥ���ޤ�����]>
!<[!UL record!%S found] >
.BASE 195
REMPACKET/FAO=1/INFORMATIONAL-
<�Х��åȤ�������ޤ����� !UL �򥿥��פ��Ʋ�������>
!<Packet received, type !UL. >
.BASE 196
COLLECTIONS/FAO=0/INFORMATIONAL-
<���쥯�����:>
!<Collections: >
.BASE 197
NOESTCOLL/FAO=0/INFORMATIONAL-
<���ꤵ�줿���쥯�����Ϥ���ޤ���>
!<No established collections. >
.BASE 198
NOSETSUSE/FAO=0/INFORMATIONAL-
<���ߤɤΥ��åȤ���ѤǤ��ޤ���>
!<No sets are currently useful. >
.BASE 199
BACKTRACE/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 200
VIRTUALFLD/FAO=0/INFORMATIONAL-
<���ۥե�����ɡ�>
!<Virtual field. >
.BASE 201
SHOCOLCOL/FAO=1/INFORMATIONAL-
<���쥯����� !AC >
!<Collection !AC >
.BASE 202
SHOCOLDOM/FAO=1/INFORMATIONAL-
<  �ɥᥤ��: !AC >
!<    Domain: !AC >
.BASE 203
SHOCOLNREC/FAO=1/INFORMATIONAL-
<           �쥳���ɿ�: !SL >
!<    Number of Records: !SL >
.BASE 204
SHOCOLSRN/FAO=1/INFORMATIONAL-
< ���򤵤줿�쥳����: !SL >
!<    Selected Record: !SL >
.BASE 205
SHOCOLSRNE/FAO=1/INFORMATIONAL-
<   ���򤵤줿�쥳����: !SL�ʺ����>
!<    Selected Record: !SL (Erased) >
.BASE 206
SHOCOLSRND/FAO=1/INFORMATIONAL-
<    ���򤵤줿�쥳����: !SL�������>
!<    Selected Record: !SL (Dropped) >
.BASE 207
SHOCOLNSR/FAO=0/INFORMATIONAL-
<    ���򤵤줿�쥳���ɤϤ���ޤ���>
!<    No Selected Record >
.BASE 209
VARIABLES/FAO=0/INFORMATIONAL-
<�������Х��ѿ�>
!<Global variables >
.BASE 210
BADMISLEN/FAO=0/WARNING-
<�Խ�ʸ�����Ķ��ʬ�ϼ����ޤ���>
!<Missing value edit string longer than edit string. >
.BASE 211
REPNOTFIT/FAO=0/ERROR-
<�������֥������Ȥ�Ĺ�����ƽ��ϹԤ������ڤ�ޤ���>
!<Print object(s) too long to fit on report line. >
.BASE 213
MAXLINEXC/FAO=0/ERROR-
<����Կ���Ķ�� - ��ݡ��Ƚ��Ϥ���ߤ��ޤ�����>
!<Maximum line count exceeded - report terminated. >
.BASE 214
NEWPINBOT/FAO=0/ERROR-
<AT TOP �ޤ��� BOTTOM OF PAGE �����ꥹ����Ǥ�!/NEW-PAGE �� NEW-SECTION �ϻ��ѤǤ��ޤ���>
!<NEW-PAGE and NEW-SECTION are illegal in AT TOP or BOTTOM OF PAGE print list. >
.BASE 215
BADQHDR/FAO=0/ERROR-
<��ݡ���̾������������ޤ���>
!<Illegal report name. >
.BASE 2�                                                                                                                                                                                                                                                                           s��} $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         S "     4       16
SHOCOLSOR/FAO=2/INFORMATIONAL-
<      �����Ƚ�: !AD >
!<    Sort order: !AD >
.BASE 217
SHOCOLSR2/FAO=2/INFORMATIONAL-
<                !AD >
!<                !AD >
.BASE 219
MAXPAGEXC/FAO=0/ERROR-
<�����ݡ��ȡ��ڡ�������Ķ�� - ��ݡ��Ƚ��Ϥ���ߤ��ޤ�����>
!<Maximum report pages exceeded - report terminated. >
.BASE 220
NOGROPRMT/FAO=0/ERROR-
<���롼�ס��ǡ������ܤؤγ����Ƥ�����������ޤ���>
!<Illegal assignment to a group data item. >
.BASE 221
INVCOLPAG/FAO=0/ERROR-
<SET COLUMNS-PAGE ���Ф��륫����������������ޤ���>
!<Invalid number of columns for Set Columns-Page. >
.BASE 222
GUIINVTER/FAO=0/ERROR-
<����ü���Ǥϥ����ɡ��⡼�ɤ����ѤǤ��ޤ���>
!<Invalid terminal type for Guide Mode. >
.BASE 223
GPRMORE/FAO=0/INFORMATIONAL-
<�ޤ�ɬ�פǤ����� ���פǤ���� N �򥿥��פ��Ʋ�����: >
!<More? (type N for no): >
.BASE 224
GPRCOMAND/FAO=0/INFORMATIONAL-
<���ޥ�ɤ����Ϥ��Ʋ����������ä����� ? �򥿥��פ��Ʋ�������>
!<enter command, type ? for help >
.BASE 225
GPRDOMAIN/FAO=0/INFORMATIONAL-
<�ɥᥤ��̾>
!<domain name >
.BASE 226
GPRRDYMOD/FAO=0/INFORMATIONAL-
"��ǥ����⡼�� �ޤ��� <return>"
!"ready mode or <return> "
.BASE 227
GPRWITH/FAO=0/INFORMATIONAL-
"WITH �ޤ��� <return>"
!"WITH or <return>"
.BASE 228
GPRFIELD/FAO=0/INFORMATIONAL-
<�ե������̾>
!<field name >
.BASE 229
GPRSHOCAT/FAO=0/INFORMATIONAL-
<���ƥ���>
!<category >
.BASE 230
GPRCOLECT/FAO=0/INFORMATIONAL-
<���쥯�����̾>
!<collection name >
.BASE 231
GUILEAVE/FAO=0/INFORMATIONAL-
<��������ʬ�Ǥ�äƤߤޤ��礦������ФäƤ��������͡�>
!<You're on your own now.  Good luck! >
.BASE 232
ADTBADKEY/FAO=0/SEVERE-
<ADT HELP �ե�����ι�¤������������ޤ���>
!<Invalid ADT HELP File Structure. >
.BASE 233
ADTAKBOVF/FAO=0/ERROR-
<ADT �κ��祭������ۤ��ޤ�����>
!<Maximum key count for ADT exceeded. >
.BASE 234
ADTANBOVF/FAO=0/ERROR-
<̾���ơ��֥롦�����Хե���>
!<Name Table Overflow. >
.BASE 235
ADTEXIT/FAO=0/ERROR-
<�桼���ؼ��ˤ�� ADT ��λ���ޤ�����>
!<ADT exited by user request. >
.BASE 236
TARRECDRO/FAO=0/INFORMATIONAL-
<�������åȡ��쥳���ɤ����� DROP ����Ƥ��ޤ���>
!<Target record has already been dropped. >
.BASE 237
GHEMODIFY/FAO=0/INFORMATIONAL-
<���򤵤줿�쥳���ɤΥե�������ͤ򹹿����ޤ� >
!<Update field values for the selected record >
.BASE 238
GHEPRINT/FAO=0/INFORMATIONAL-
<���Ĥޤ���ʣ���Υ쥳���ɤ�������ޤ� >
!<Print a record or records >
.BASE 239
GHESORT/FAO=0/INFORMATIONAL-
<���ߤΥ��쥯������ƹ������ޤ� >
!<Re-arrange the current collection >
.BASE 240
GHESELECT/FAO=0/INFORMATIONAL-
<����Υ쥳���ɤ����Ӥޤ� >
!<Pick out a particular record >
.BASE 241
GHEFIND/FAO=0/INFORMATIONAL-
<�쥳���ɤΥ��쥯�����򸡺����ޤ� >
!<Retrieve a collection of records >
.BASE 242
GHESTORE/FAO=0/INFORMATIONAL-
<�ɥᥤ��ˣ��Ŀ������쥳���ɤ��ɲä��ޤ� >
!<Add a new record to a domain >
.BASE 243                  
GHEREADY/FAO=0/INFORMATIONAL-
<�ɥᥤ�����Ѳ�ǽ�ˤ��ޤ�>
!<Make a domain available >
.BASE 244
GHESHOW/FAO=0/INFORMATIONAL-
<���ƥ����������ɽ�����ޤ�>
!<Display status information >
.BASE 245
GHELEAVE/FAO=0/INFORMATIONAL-
<�̾�� DATATRIEVE �����ޤ�>
!<Return to normal DATATRIEVE >
.BASE 246
GUIPOSRES/FAO=0/INFORMATIONAL-
<�ʲ��Τ�Τ�������ǲ�����: >
!<Possible responses are: >
.BASE 247
GUIBRETB/FAO=0/INFORMATIONAL-
"<return>"
!"<return> "
.BASE 248
GUIBADCHR/FAO=0/INFORMATIONAL-
<���ѤǤ��ʤ�ʸ���Ǥ���? �����Ϥ�������ϲ�ǽ�ʤ�Τ�ɽ������ޤ���>
!<Unusable character, type ? to get valid responses. >
.BASE 249
GPRDOMCOL/FAO=0/INFORMATIONAL-
<�ɥᥤ��ޤ��ϥ��쥯�����̾>
!<domain or collection name >
.BASE 250
GHEDOMAIN/FAO=0/INFORMATIONAL-
<�쥳���ɡ��ɥᥤ��>
!<Record domain >
.BASE 251
GHECOLECT/FAO=0/INFORMATIONAL-
<�쥳���ɡ����쥯�����>
!<Record collection >
.BASE 252
NOFMS/FAO=0/SEVERE-
<DATATRIEVE �� FORMS �򥵥ݡ��Ȥ���褦�˥��󥹥ȡ��뤵��Ƥ��ޤ���>
!<DATATRIEVE has not been installed with forms support. >
.BASE 253
GHEWITH/FAO=0/INFORMATIONAL-
<�쥳���ɡ��������ξ�����ꤷ�ޤ�>
!<Qualify record search >
.BASE 254
GPRCOMPAR/FAO=0/INFORMATIONAL-
<���>
!<comparison >
.BASE 255
GHEALPHA/FAO=0/INFORMATIONAL-
<�ѿ������ե������>
!<Alpha/numeric field >
.BASE 256
GHEALPHAQ/FAO=2/INFORMATIONAL-
<�ѿ������ե������ (!AC ����̾��>
!<Alpha/numeric field (alias for !AC) >
.BASE 257
GHENUMBR/FAO=0/INFORMATIONAL-
<�����ե������>
!<Numeric field >
.BASE 258
GHENUMBRQ/FAO=2/INFORMATIONAL-
<�����ե������ (!AC ����̾��>
!<Numeric field (alias for !AC) >
.BASE 259
GHEDATE/FAO=0/INFORMATIONAL-
<���եե������>
!<Date field >
.BASE 260
GHEDATEQ/FAO=2/INFORMATIONAL-
<���եե������ (!AC ����̾��>
!<Date field (alias for !AC) >
.BASE 261
GHECOMPU/FAO=0/INFORMATIONAL-
<�׻����줿��>
!<A computed value >
.BASE 262
GHECOMPUQ/FAO=2/INFORMATIONAL-
<�׻����줿�� (!AC ����̾��>
!<A computed value (alias for !AC) >
.BASE 263
GHELIST/FAO=0/INFORMATIONAL-
<�ꥹ��>
!<List >
.BASE 264
GHELISTQ/FAO=2/INFORMATIONAL-
<�ꥹ�� (!AC ����̾��>
!<List (alias for !AC) >
.BASE 265
GHEGROUP/FAO=0/INFORMATIONAL-
<���롼�ס��ե������>
!<Group field >
.BASE 266
GHEGROUPQ/FAO=2/INFORMATIONAL-
<���롼�ס��ե������ (!AC ����̾��>
!<Group field (alias for !AC) >
.BASE 267
GUIEXECUT/FAO=0/INFORMATIONAL-
<[�¹���Ǥ�...]>
!<[Executing...] >
.BASE 268
GUILEAVNG/FAO=0/INFORMATIONAL-
<��������ʬ�Ǥ�äƤߤޤ��礦��>
!<You're on your own now.  Good luck! >
.BASE 269
GPRQUOTE/FAO=0/INFORMATIONAL-
<ʸ����ϰ�����Ǥ��᤯���äƲ�������>
!<character string, end with a quote >
.BASE 270
GPRCOMRET/FAO=0/INFORMATIONAL-
"����� �ޤ��� <return> "
!"comma or <return>"
.BASE 271
GPRFLDRET/FAO=0/INFORMATIONAL-
"�ե������̾�ꥹ�Ȥޤ��� <return>"
!"fieldname list or <return> "
.BASE 272
GPRALLFLD/FAO=0/INFORMATIONAL-
"ALL, �ե������̾�ꥹ�Ȥޤ��� <return>"
!"ALL, fieldname list or <return> "
.BASE 273
CHAOPTINV/FAO=0/ERROR-
<�ե�����Σ��������Ǥ� "CHANGE" ���ץ����ϻ��ѤǤ��ޤ���>
!<The "CHANGE" option is invalid on the primary key of a file. >
.BASE 274
LOADEDTAB/FAO=1/INFORMATIONAL-
<!/�����ɤ��줿�ơ��֥�: >
!<!/Loaded tables: >
.BASE 275
NOLOADTAB/FAO=0/INFORMATIONAL-
<�����ɤ��줿�ơ��֥�Ϥ���ޤ���>
!<No loaded tables. >
.BASE 276
GHEALL/FAO=0/INFORMATIONAL-
<���ߤΥ��쥯�����Υ쥳������������Ѥ��ޤ�>
!<Use all of the records in the current collection >
.BASE 277
GPRFLDDEC/FAO=0/INFORMATIONAL-
<�ե������̾�ޤ��� DECREASING >
!<fieldname or DECREASING >
.BASE 278
GHEDECREA/FAO=0/INFORMATIONAL-
<�ո����˥�����>
!<Sort backwards >
.BASE 279
SHOSETUP/FAO=0/INFORMATIONAL-
<���åȥ��å�:>
!<Set-up: >
.BASE 280
SHOCOLPAG/FAO=1/INFORMATIONAL-
<  ����ࡦ�ڡ���: !UL >
!<    Columns-page: !UL >
.BASE 281
SHONOABO/FAO=0/INFORMATIONAL-
<    ������λ���ʤ�>
!<    No abort >
.BASE 282
SHOABORT/FAO=0/INFORMATIONAL-
<    ������λ����>
!<    Abort >
.BASE 283
SHONOFOR/FAO=0/INFORMATIONAL-
<    �ե�����򤷤ʤ�>
!<    No form >
.BASE 284
SHOFORM/FAO=0/INFORMATIONAL-
<    �ե�����򤹤�>
!<    Form >
.BASE 285
SHONOSEA/FAO=0/INFORMATIONAL-
<    ���������ʤ�>
!<    No search >
.BASE 286
SHOSEARCH/FAO=0/INFORMATIONAL-
<    ����������>
!<    Search >
.BASE 287
SHONOPRO/FAO=0/INFORMATIONAL-
<    �ץ���ץȤ�Ф��ʤ�>
!<    No prompt >
.BASE 288
SHOPROMPT/FAO=0/INFORMATIONAL-
<    �ץ���ץȤ�Ф�>
!<    Prompt >
.BASE 289
GHERETURN/FAO=0/INFORMATIONAL-
<ʸ��λ���ޤ�>
!<Finish statement >
.BASE 290
GHENEXT/FAO=0/INFORMATIONAL-
<���쥯�����μ��Υ쥳���ɡʾ�ά�͡�>
!<Next record of the collection (default case) >
.BASE 291
GHEPRIOR/FAO=0/INFORMATIONAL-
<���쥯���������Υ쥳����>
!<Previous record of the collection >
.BASE 292
GPRSELOPT/FAO=0/INFORMATIONAL-
"�ɤΥ쥳���ɤޤ��� <return>"
!"which record or <return> "
.BASE 293
GHEGE/FAO=0/INFORMATIONAL-
<����礭����������>
!<Greater than or equal to >
.BASE 294
GHEGT/FAO=0/INFORMATIONAL-
<����礭��>
!<Greater than >
.BASE 295
GHELE/FAO=0/INFORMATIONAL-
<��꾮������������>
!<Less than or equal to >
.BASE 296
GHELT/FAO=0/INFORMATIONAL-
<��꾮����>
!<Less than >
.BASE 297
GHECONTAI/FAO=0/INFORMATIONAL-
<��ʬʸ�����ޤ�>
!<Containing a substring >
.BASE 298
GHEREADYR/FAO=0/INFORMATIONAL-
<�ɼ�ꥢ�������Τ�>
!<Read access only >
.BASE 299
GHEREADYM/FAO=0/INFORMATIONAL-
<�ɼ��ڤӽ������������Τ�>
!<Read and modify access only >
.BASE 300
GHEREADYW/FAO=0/INFORMATIONAL-
<�ɼ��, ����, �񤭹��ߥ�������>
!<Read, modify and store access >
.BASE 301
EXPINVPIC/FAO=0/ERROR-
<PICTURE ��ˤϻؿ����Խ�ʸ���󤬻��ѤǤ��ޤ���>
!<Exponential edit-string is invalid for PICTURE clause. >
.BASE 302
CONTPREQ/FAO=0/ERROR-
<PICTURE �ޤ��� EDIT ʸ����Ǥ� "P" �Ϥ��٤����ܤ��Ƥ��ʤ���Фʤ�ޤ���>
!<All "P"s must be contiguous in PICTURE or EDIT string. >
.BASE 303
ONEEPERM/FAO=0/ERROR-
<PICTURE �ޤ��� EDIT ʸ����Ǥϣ��Ĥ� "E" �Τߤ�������ޤ���>
!<Only one "�                                                                                                                                                                                                                                                                           ��op $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         �� "     E       E" permitted in PICTURE or EDIT string. >
.BASE 304
NOTDBNAM/FAO=1/ERROR-
<"!AS" �ϥǡ����١���̾�ǤϤ���ޤ���>
!<"!AS" is not a database name. >
.BASE 305
DATABASES/FAO=0/INFORMATIONAL-
<�ǡ����١���: >
!<Databases: >
.BASE 306
GHEANY/FAO=0/INFORMATIONAL-
<���������ꥹ��>
!<Search list >
.BASE 307
GPRLIST/FAO=0/INFORMATIONAL-
<�ꥹ��>
!<list >
.BASE 308
NOCTXCON/FAO=2/ERROR-
<"!AD" �Τ����ʸ̮�����Ĥ���ޤ���>
!<No context found for "!AD". >
.BASE 309
NODBMS/FAO=0/ERROR-
<DBMS ���󥿥ե����������Υ��᡼���˴ޤޤ�Ƥ��ޤ���>
!<The DBMS interface is not included in this image. >
.BASE 310
GUINXTCMD/FAO=0/INFORMATIONAL-
<���Υ��ޥ�ɤ�ɤ���: >
!<Ready for next command >
.BASE 311
BADSTRDES/FAO=0/ERROR-
<ʸ���󵭽һҤ�����������ޤ���>
!<Invalid string descriptor. >
.BASE 312
NOGLOVAR/FAO=0/INFORMATIONAL-
<�������Х��ѿ����������Ƥ��ޤ���>
!<No global variables are declared. >
.BASE 313
DUPUSACLA/FAO=2/ERROR-
<���ꤵ�줿 USAGE �᤬��ʣ���Ƥ��ޤ�: "!AD" >
!<Duplicate USAGE clause specified: "!AD". >
.BASE 314
ILLASCNUM/FAO=1/WARNING-
<ASCII ���� "!AS" �ϻ��ѤǤ��ޤ���>
!<Illegal ASCII numeric "!AS". >
.BASE 315
SHOPRIVR/FAO=0/INFORMATIONAL-
< R (DTR_READ)		- READ �Ѥ� READY �Ǥ��ޤ���SHOW �� EXTRACT ����Ѥ��Ʋ�������>
!< R (DTR_READ)		- may ready for READ, use SHOW and EXTRACT >
.BASE 316
SHOPRIVW/FAO=0/INFORMATIONAL-
< W (DTR_WRITE)		- READ, WRITE, MODIFY, EXTEND �Ѥ� READY �Ǥ��ޤ���>
!< W (DTR_WRITE)	- may ready for READ, WRITE, MODIFY, or EXTEND >
.BASE 317
SHOPRIVM/FAO=0/INFORMATIONAL-
< M (DTR_MODIFY)	- READ, MODIFY �Ѥ� READY �Ǥ��ޤ���>
!< M (DTR_MODIFY)	- may ready for READ, MODIFY >
.BASE 318
SHOPRIVE/FAO=0/INFORMATIONAL-
< E (DTR_EXTEND/EXECUTE) - EXTEND �Ѥ� READY �Ǥ��ޤ����ޤ��ơ��֥롿�ץ���������˥��������Ǥ��ޤ���>
!< E (DTR_EXTEND/EXECUTE) - may ready to EXTEND, or access table or procedure >
.BASE 319
SHOPRIVC/FAO=0/INFORMATIONAL-
< C (CONTROL)		- DEFINEP, SHOWP, DELETEP ���ޥ�ɤ��Ȥ��ޤ���>
!< C (CONTROL)		- may issue DEFINEP, SHOWP, DELETEP commands >
.BASE 320                                                           
SHOPRIVD/FAO=0/INFORMATIONAL-
< D (LOCAL_DELETE)	- ���񥪥֥������Ȥ����Ǥ��ޤ���>
!< D (LOCAL_DELETE)	- may delete a dictionary object >
.BASE 321
SHOPRIVF/FAO=0/INFORMATIONAL-
< F (FORWARD)		- ���ּ��������Ǥ��ޤ���>
!< F (FORWARD)		- may create a subdictionary >
.BASE 322
SHOPRIVG/FAO=0/INFORMATIONAL-
< G (GLOBAL_DELETE)	- ����Ȥ��λ�¹�����Ǥ��ޤ���>
!< G (GLOBAL_DELETE)	- may delete a directory and its descendents 
.BASE 323
SHOPRIVH/FAO=0/INFORMATIONAL-
< H (HISTORY)		- ���֥������ȤΥҥ��ȥꡦ�ꥹ�Ȥ˹��ܤ��ɲäǤ��ޤ���>
!< H (HISTORY)		- may add entries to object's history list >
.BASE 324
SHOPRIVP/FAO=0/INFORMATIONAL-
< P (PASS_THRU)        - �ǥ��쥯�ȥ��Ϳ����줿̾���ޤ��ϥѥ�̾�Υ��֥������Ȥ���ѤǤ��ޤ���>
!< P (PASS_THRU)        - may use given name of directory or object in pathname >
.BASE 325
SHOPRIVS/FAO=0/INFORMATIONAL-
< S (SEE)		- ����򸫤���ɤ�ˤ��Ȥ��Ǥ��ޤ���>
!< S (SEE)		- may see (read) dictionary >
.BASE 326
SHOPRIVU/FAO=0/INFORMATIONAL-
< U (UPDATE)		- ���񥪥֥������Ȥ򹹿��Ǥ��ޤ���>
!< U (UPDATE)		- may update dictionary object >
.BASE 327
SHOPRIVX/FAO=0/INFORMATIONAL-
< X (EXTEND)		- �ǥ��쥯�ȥ�ޤ��ϥǥ��쥯�ȥ���Υ��֥������Ȥ�����Ǥ��ޤ���>
!< X (EXTEND)		- may create directory or object within directory >
.BASE 328
SHOPRIVNO/FAO=0/INFORMATIONAL-
<�ǥ��쥯�ȥ�ޤ��ϼ��񥪥֥������Ȥ��Ф����ø�������ޤ���>
!< No privileges to directory or dictionary object. >
.BASE 329
FILNOTFND/FAO=1/ERROR-
<�ե����뤬���Ĥ���ޤ��� - "!AC"��>
!<File not found - "!AC". >
.BASE 330
SELPURCOL/FAO=2/INFORMATIONAL-
<�ƥ��쥯���������򤬲򤫤졤���쥯����� !AD ����ưŪ�˲�������ޤ�����>
!<Parent collection de-selected, collection !AD automatically released. >
.BASE 331
PROVERMIS/FAO=2/SEVERE-
<ʬ���ץ��ȥ��롦�С��������԰��ס��ۥ��� = !UL  ������ = !UL >
!<Distributed protocol version mismatch. Host = !UL Server = !UL >
.BASE 332
ROLLBACK/FAO=0/WARNING-
<COMMIT �����Ԥ�����ư ROLLBACK ���ʹ���Ǥ���>
!<COMMIT failed, automatic ROLLBACK in progress. >
.BASE 333
NOREMCROS/FAO=0/ERROR-
<CROSS��ˤϱ�֥ɥᥤ������Ǥ��ޤ���>
!<A remote domain may not participate in a CROSS. >
.BASE 334
ILLNEGNUM/FAO=0/WARNING-
<̵��̣�ʥޥ��ʥ��͡������Ȥ��ޤ���>
!<Meaningless negative value; zero used instead. >
.BASE 335
TOOMANSIG/FAO=1/WARNING-
<ʸ���� "!AS" ����椬¿������Τǡ��ǽ�Τ�Τ��������̵�뤷�ޤ���>
!<Too many signs in string "!AS", ignoring all but first. >
.BASE 336
TOOMANDEC/FAO=1/WARNING-
<ʸ���� "!AS" �ξ�������¿������Τǡ��ǽ�Τ�Τ��������̵�뤷�ޤ���>
!<Too many decimal points in string "!AS", ignoring all but first. >
.BASE 337
DATINPERR/FAO=1/WARNING-
<DTR$DATE_INPUT���������顼���������줿�� "!AS"�ˡ�>
!<Error translating DTR$DATE_INPUT (translated value "!AS"). >
.BASE 338
NOVIEWSTO/FAO=1/ERROR-
<�ӥ塼����� STORE ���뤳�ȤϤǤ��ޤ���>
!<Illegal STORE into a view. >
.BASE 340
CANRDYDOM/FAO=1/ERROR-
<COMMIT, ROLLBACK �ޤ��� FINISH ���Ф���ʤ��¤�!/���Υ����������⡼�ɤǤϥɥᥤ��� READY �Ǥ��ޤ���>    
!<Source cannot be readied at this access mode !/unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 341
CANRRDYDM/FAO=1/WARNING-
<COMMIT, ROLLBACK �ޤ��� FINISH ���Ф���ʤ��¤�!/���Υ����������⡼�ɤǤϥɥᥤ���� READY �Ǥ��ޤ���>
!<Source cannot be re-readied at this access mode !/unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 342
BADFLDCTX/FAO=2/ERROR-
<�ե������ "!AD" ��ʸ̮������������ޤ���>
!<Invalid context for field "!AD". >
.BASE 343
OCCREQFRO/FAO=0/ERROR-
<OCCURS ��ˤϤ���㤤��٥�Ǿ��ʤ��Ȥ⣱�Ĥ� FROM �ե�����ɤ�ɬ�פǤ���>
!<OCCURS clause requires at least one FROM field at a lower level. >
.BASE 344
SHOWTEXT/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 345
NOSTOREL/FAO=1/ERROR-
<���ƥ��֡��ե������ STORE �Ǥ��ޤ��󡣡ʥɥᥤ�� !AC)��>
!<Can't STORE into a relative file (domain !AC). >
.BASE 346
NOERASEQ/FAO=1/ERROR-
<�������󥷥�롦�ե����뤫�� ERASE �Ǥ��ޤ���ʥɥᥤ�� !AC�ˡ�>
!<Can't ERASE from a sequential file (domain !AC). >
.BASE 347
NOREMANY/FAO=0/ERROR-
<��� ANY �ޤ��� IN ɽ���ϥ��ݡ��Ȥ���Ƥ��ޤ���>
!<Remote ANY or IN expressions are not supported. >
.BASE 348
SHOWPROC/FAO=2/INFORMATIONAL-
<PROCEDURE !AD >
!<PROCEDURE !AD >
.BASE 349
NOERALIS/FAO=0/ERROR-
<OCCURS �ꥹ�Ȥ������Ǥ��ޤ���>
!<Can't ERASE from an OCCURS list. >
.BASE 350
NOERACRO/FAO=0/ERROR-
<CROSS �������Ǥ��ޤ���>
!<Can't ERASE from a CROSS. >
.BASE 351
NOREMEXP/FAO=0/ERROR-
<���ɽ���ϥ��ݡ��Ȥ���Ƥ��ޤ���>
!<Remote expression is not supported. >
.BASE 352
ASSNEGUNS/FAO=0/WARNING-
<���̵�����ܤ��Ф�������ͤ�������Ƥ��Ƥ��ޤ�: �����ͤ���Ѥ��ޤ���>
!<Assignment of negative value to unsigned item; absolute value used. >
.BASE 353
MAGICFAIL/FAO=0/INFORMATIONAL-
<����ƥ����ȡ��������㡼���ե�����ɤ��褷�褦�Ȥ��Ƽ��Ԥ��ޤ�����>
!<Context Searcher failed trying to resolve field. >
.BASE 354
BADDATEOP/FAO=0/ERROR-
<���դϳݤ����ޤ��ϳ�껻�Ǥ��ޤ���>
!<Cannot multiply or divide dates. >
.BASE 355
GUINOMORE/FAO=0/INFORMATIONAL-
<�⤦����ޤ���>
!<No more >
.BASE 356
SELBADBOO/FAO=0/ERROR-
<�֡�������������쥳���ɤ�����Ǥ��ޤ���>
!<Unable to select a record that satisfies the Boolean. >
.BASE 357
SIGNON_FT/FAO=5/INFORMATIONAL-
<���ܸ� DEC DATATRIEVE !AD!UL.!UL-!UL!/���ܣģţø�����𥷥��ƥ�!/HELP �����Ϥ���ȥإ�פ�Ȥ��ޤ���!/>
!<DEC DATATRIEVE !AD!UL.!UL-!UL!/Digital Query and Report System!/Type HELP for help !/>
.BASE 358
NAMTOOLONG/FAO=0/ERROR-                                                                                        
<�ե������̾�ϣ���ʸ�����Ĺ���Ȥ�ޤ���>
!<Field name can't be longer than 31 characters. >
.BASE 359
GPRCONT/FAO=0/INFORMATIONAL-
<³������Ϥɤ줫�����򲡤��Ʋ�������>
!<Type any character to continue: >
.BASE 360
NOEXTRACT/FAO=0/ERROR-
<���Υ쥳���ɤˤ� DATATRIEVE �����ɽ���Ǥ��ʤ���Τ��ޤޤ�Ƥ��ޤ���>
!<That record contains things that can't be expressed in the DATATRIEVE language. >
.BASE 361
SORTOPEN/FAO=0/ERROR-
<�����ȤΤ���κ�ȥե����������Υ��顼�Ǥ���>
!<Error creating the work files for the sort. >
.BASE 362
HELPINIT/FAO=0/ERROR-
<�إ�ס��饤�֥�������������˥��顼��ȯ�����ޤ�����>
!<An error occured while trying to initialize the HELP library. >
.BASE 363
HELPOPEN/FAO=0/ERROR-
<�إ�ס��饤�֥��򥪡��ץ󤹤���˥��顼��ȯ�����ޤ�����>
!<An error occured while trying to open the HELP library. >
.BASE 364
SETNOTUSE/FAO=2/ERROR-
<���å� "!AD" �ϻ��ѤǤ��ޤ��� - ��ͭ�Ԥޤ��ϥ��С����ɥᥤ�� READY ����Ƥ��ޤ���>
!<Set "!AD" cannot be used - either an owner or a member source is not ready. >
.BASE 365
BADPORTOP/FAO=0/ERROR-�                                                                                                                                                                                                                                                                           ]�5 $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         jf "     V       
<�ݡ��Ȥ� MODIFY, ERASE, FIND, SORT, REDUCE ���뤳�ȤϤǤ��ޤ���>
!<It is invalid to MODIFY, ERASE, FIND, SORT or REDUCE a port. >
.BASE 366
TOOMNYFLD/FAO=0/ERROR-
<�ե�����ɿ������٤�ۤ��ޤ�����>
!<Maximum number of fields exceeded. >
.BASE 367
GIVNAMUSE/FAO=3/ERROR-
<Ϳ����줿̾�� "!AD" �ϥɥᥤ�� !AC �ˤ����˻��Ѥ���Ƥ��ޤ���>
!<Given name "!AD" is already used by source !AC. >
.BASE 368
FLDSELFREF/FAO=2/ERROR-
<�ե������ "!AD" �ϼ��ʻ��Ȥ�ޤ�Τ�ɾ���Ǥ��ޤ���>
!<Field "!AD" contains a self-reference and cannot be evaluated. >
.BASE 369
OCCDEPOCC/FAO=0/ERROR-
<OCCURS DEPENDING ON �� OCCURS �ꥹ����ˤϻȤ��ޤ���>
!<An OCCURS DEPENDING ON is not allowed within an OCCURS list. >
.BASE 371
PURETEST/FAO=0/INFORMATIONAL-
<�����ñ�ʤ�ƥ��ȤǤ���>
!<This is purely a test >
.BASE 373
MINE/FAO=2/INFORMATIONAL-
<������ �����ǥ��󥰡����顼 ������>
!<Internal coding error. >
.BASE 374
NOPROJFLD/FAO=0/ERROR-
<�����ξ��ʤ��Ȥ⣱�Ĥ��쥳�������������줿�ե�����ɤǤʤ���Фʤ�ޤ���>
!<At least one reduce key must be a field defined in the record.>
.BASE 375
NOCOLPRO/FAO=0/ERROR-
<REDUCE ʸ���Ф��륳�쥯����󤬤���ޤ���>
!<No collection for REDUCE statement. >
.BASE 376
OPENFILE/FAO=2/INFORMATIONAL-
<�ե����� !AD �������Ǥ���>
!<Creating file !AD ... >
.BASE 377
OPENTERM/FAO=2/INFORMATIONAL-
<ü�� !AD �˽��Ϥ�������Ǥ���>
!<Sending output to terminal !AD. >
.BASE 378
OPENLP/FAO=2/INFORMATIONAL-
<!AD �˽��Ϥ򥹥ס��뤷�Ƥ��ޤ���>
!<Spooling output to !AD. >
.BASE 379
ELSEREQ/FAO=0/ERROR-
<IF �� CHOICE ���ͤˤ� ELSE �᤬ɬ�פǤ���>
!<An ELSE clause is required for IF and CHOICE values. >
.BASE 380
ENDOFSTR/FAO=0/WARNING-
<ʸ����ν��� - �桼�����������ɤ��Ф��ƻ��ѤǤ���ȡ����󤬤���ޤ���>
!<End of string - no more tokens available for user defined keyword. >
.BASE 381
MORESTR/FAO=0/INFORMATIONAL-
<�桼�����������ɤ��Ф��Ƥޤ��ȡ����󤬻��ѤǤ��ޤ���>
!<There are more tokens available for user defined keyword. >
.BASE 382
TRUNCSTR/FAO=0/WARNING-
<�ȡ�������ڤ�ΤƤޤ��������Υ������;�꤬�֤���ޤ���>
!<Truncated a token.  Remainder returned on next call. >
.BASE 383
VIDEOHELP/FAO=0/INFORMATIONAL-
" H E L P  -  ���ä����� <PF2>�����ܤ��ѹ��ˤ� <return> �����Ϥ��Ʋ�����"
!" H E L P  -  type <PF2> for help.  Type <RETURN> to change help topics. "
.BASE 384
SHOWHELP/FAO=4/INFORMATIONAL-
< �إ�פ�����: >
!<Help Settings: >
.BASE 385
HELPLINES/FAO=2/INFORMATIONAL-
<!UL ���� !UL �ޤǤι�>
!<    Lines !UL to !UL >
.BASE 386
BADHLPLIN/FAO=2/WARNING-
<�إ�׹��ֹ椬�ϰϳ��Ǥ��뤫��ʬ�ʹԤǤϤ���ޤ��󡣥ȥå�: !SL���ܥȥ�: !SL >
!<Help line number out of range or not enough lines. Top: !SL  Bottom: !SL. >
.BASE 387
NOTINFDI/FAO=0/ERROR-
<GET_FORM �� PUT_FORM �� DISPLAY_FORM ʸ��ǤΤߵ�����ޤ���>
!<GET_FORM and PUT_FORM are allowed only in DISPLAY_FORM statements. >
.BASE 388
NESTFDI/FAO=0/ERROR-
<DISPLAY_FORM ��� DISPLAY_FORM ʸ��ͥ��Ȥ��뤳�ȤϤǤ��ޤ���>
!<You cannot nest DISPLAY_FORM statements within DISPLAY_FORM. >
.BASE 389
INVNODSPC/FAO=0/ERROR-
<�Ρ��ɻ��꤬����������ޤ���>
!<Invalid Node Specification. >
.BASE 390
SYNALDEC/FAO=2/WARNING-
<"!AD" �ϥ�����ɤǤ��뤫��Ʊ����Ȥ��ƴ����������Ƥ��ޤ���>
!<"!AD" is a keyword or is already declared as a synonym. >
.BASE 391
NODISCO/FAO=0/ERROR-
<CONNECT, DISCONNECT, �ڤ� RECONNECT �ϱ�֥ɥᥤ��ˤϻ��ѤǤ��ޤ���>
!<CONNECT, DISCONNECT, and RECONNECT don't work on remote domains. >
.BASE 392
GPRPLOT/FAO=0/INFORMATIONAL-
<�ץ��å�̾>
!<plot name >
.BASE 393
GHEEDIT/FAO=0/INFORMATIONAL-
<DATATRIEVE ���ǥ����ˤϤ���ޤ�����>
!<Enter the DATATRIEVE editor >
.BASE 394
GPRPROC/FAO=0/INFORMATIONAL-
<�ץ���������̾>
!<procedure name >
.BASE 395
GHEPROC/FAO=0/INFORMATIONAL-
<DATATRIEVE �ץ����������ƤӽФ��ޤ�>
!<Invoke a DATATRIEVE procedure >
.BASE 396
GHEPLOT/FAO=0/INFORMATIONAL-
<���ߤΥ��쥯����󤫤�ǡ����򥰥�դˤ��ޤ�>
!<Graph data from the current collection >
.BASE 397
GHECROSS/FAO=0/INFORMATIONAL-
<���Υ��������̤Υ쥳���ɡ��������� JOIN ���ޤ�>
!<Join another record source to prior sources >
.BASE 398
GHEOVER/FAO=0/INFORMATIONAL-
<���̥ե�����ɤ���Ѥ��ƥ쥳���ɡ����������Ϣ�Ť��ޤ�>
!<Associate record sources using a common field >
.BASE 399
GPRCROSS/FAO=0/INFORMATIONAL-
"CROSS, WITH �ޤ��� <return>"
!"CROSS, WITH or <return> "
.BASE 400
GPROVER/FAO=0/INFORMATIONAL-
"CROSS, OVER, WITH �ޤ��� <return>"
!"CROSS, OVER, WITH or <return> "
.BASE 401
BADUDKCTX/FAO=0/ERROR-
<�桼�����������ɤ˻��ꤵ�줿�ѡ�����ʸ̮������������ޤ���>
!<Invalid parsing context specified for User Defined Keyword. >
.BASE 402
BADUDKIDX/FAO=0/ERROR-
<�桼�����������ɤ˻��ꤵ�줿����ǥå���������������ޤ���>
!<Invalid index specified for User Defined Keyword. >
.BASE 403
UDKUNWIND/FAO=0/INFORMATIONAL-
<UDK �����򣱥�٥������ᤷ�ޤ���>
!<Unwind UDK processing one level. >
.BASE 404
BADTOKTYP/FAO=0/ERROR-
<�ꥯ�����Ȥ��줿�ȡ����󡦥����פ�����������ޤ���>
!<Invalid token type requested. >
.BASE 405
GPRNUMBER/FAO=0/INFORMATIONAL-
<�ֹ�>
!<number >
.BASE 406
BADSTALL/FAO=0/SEVERE-
<DAB �Υ��ȡ��롦�ݥ���Ȥ�����������ޤ���>
!<Stall point in DAB is invalid. >
.BASE 407
EXIT/FAO=0/SUCCESS-
<���ڥ졼���λؼ��ˤ�� DATATRIEVE ��λ���ޤ���>
!<Exit from DATATRIEVE requested by operator. >
.BASE 408
INPLINTRU/FAO=0/ERROR-
<���ϹԤ�Ĺ�����ޤ� - 255 ʸ�����ڤ�ΤƤޤ�����>
!<Input line too long - truncated to 255 characters. >
.BASE 409
NOPGETPUT/FAO=0/ERROR-
<�ݡ��ȤǤγ�Ǽ�����������ü� DATATRIEVE �Ǥϵ�����Ƥ��ޤ���>
!<Storing and retrieving from ports is not allowed in interactive DATATRIEVE. >
.BASE 410
NOUDKS/FAO=0/ERROR-
<�桼�����������ɤ����ü� DATATRIEVE �Ǥϵ�����Ƥ��ޤ���>
!<User Defined Keywords are not allowed in interactive DATATRIEVE. >
.BASE 411
NOCOMFILE/FAO=0/ERROR-
<���ܥ��ޥ�ɡ��ե�����̾������٤��ʤΤˡ��Ԥ����äƤ��ޤ��ޤ�����>
!<Expected indirect command file name, encountered end of line. >
.BASE 412
GPROPER/FAO=0/INFORMATIONAL-
<���ץ���ʥ�ʻ��ѱ黻��>
!<Optional arithmetic operator >
.BASE 413
NOLOGFILE/FAO=0/ERROR-
<�������ե�����̾������٤��ʤΤˡ��Ԥ����äƤ��ޤ��ޤ�����>
!<Expected log file name, encountered end of line. >
.BASE 414
REMCMD/FAO=2/INFORMATIONAL-
"REMCMD> !AD "
!"REMCMD> !AD "
.BASE 415
SHONOVERI/FAO=0/INFORMATIONAL-
<    ��ǧ���ʤ�>
!<    No verify >
.BASE 416
SHOVERIFY/FAO=0/INFORMATIONAL-
<    ��ǧ����>
!<    Verify >
.BASE 417
HLPNOPMPT/FAO=0/INFORMATIONAL-
<    �ץ���ץȤ�Ф��ʤ�>
!<    No prompting >
.BASE 418
HLPPROMPT/FAO=0/INFORMATIONAL-
<    �ץ���ץȤ�Ф�>
!<    Prompting >
.BASE 419
HLPNOWNDW/FAO=0/INFORMATIONAL-
<    ������ɥ��ˤ��ʤ�>
!<    No window >
.BASE 420
HLPWINDOW/FAO=0/INFORMATIONAL-               
<    ������ɥ��ˤ���>
!<    Window >
.BASE 421
ADTENOTNULL/FAO=0/WARNING-
<���٤ƤΥ��ڡ������ġ��ޤ����֤���Ѥ��ʤ��ǲ�������>
!<Do not use all spaces and/or tabs. >
.BASE 422
ADTECANTOPEN/FAO=1/WARNING-
<ADT ������̾���Υե�����򥪡��ץ�Ǥ��ޤ���Ǥ�����!/�̤Υե�����̾�����Ϥ��Ʋ�������>
!<ADT could not open a file with that name.!/Please enter another file name. >
.BASE 423
ADTEKEYWORD/FAO=1/WARNING-
<����̾���� DATATRIEVE ������ɤȽ�ʣ���ޤ����̤�̾�������Ϥ��Ʋ�������>
!<That name duplicates a DATATRIEVE keyword.!/Please enter another name. >
.BASE 424
ADTEDUPNAME/FAO=1/WARNING-
<!AC ���̤�̾���Ƚ�ʣ���ޤ�����ʣ���ʤ�̾�������Ϥ��Ʋ�������>
!<!AC duplicates another name.!/Please enter a unique name. >
.BASE 425
ADTEINVTYPE/FAO=1/WARNING-
<DATE, PERCENT, MONEY, NUMBERS, CHARACTERS �ޤ��� GROUP �������Ʋ�������!/��ά�� D, P, M, N, C, G ����ѤǤ��ޤ���>
!<Please answer with DATE, PERCENT, MONEY, NUMBERS,!/CHARACTERS, or GROUP.!/You can also use the abbreviations D, P, M, N, C, or G. >
.BASE 426
ADTEINVFILE/FAO=1/WARNING-
<�������ե�����̾����Ѥ��Ʋ����������磹�Ĥαѻ��ޤ��Ͽ�����!/���ץ���ʥ��1�ĤΥԥꥪ�ɤȺ��磳�Ĥαѻ��ޤ��Ͽ��������Ϥ��Ʋ�������>
!<Please use a valid file name. Enter up to 9 letters or digits,!/and  optionally a period and up to 3 letters or digits. >
.BASE 427
ADTEINVDATE/FAO=0/WARNING-
<�ʲ��Υե����ޥå��ֹ�Σ��Ĥ���Ѥ��Ʋ�����: 1, 2, 3 �ޤ��� 4��>
!<Please use one of these format numbers: 1, 2, 3, or 4. >
.BASE 428
ADTEKEYDOM/FAO=1/WARNING-
<�������ե�����ɤ�̾���Ȥ��ƥɥᥤ��̾����Ѥ��ʤ��ǲ�������!/�ե�����ɤ�̾�������Ϥ��Ʋ�������>
!<Do not use the domain name as the name of the key field.!/Please enter the name of a field. >
.BASE 429
ADTENODOM/FAO=0/WARNING-
<�ɥᥤ��ޤ��ϥե�����ɤ��������������Ƥ��ޤ���>
!<You have not defined any domains or fields. >
.BASE 430
ADTENOTNUM/FAO=0/WARNING-
<���Ĥ������������Ʋ�������>
!<Please answer with an integer. >
.BASE 431
ADTENOTPOS/FAO=0/WARNING-
<��������礭�����������Ʋ�������>
�                                                                                                                                                                                                                                                                           O�{� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         [� "     g       !<Please answer with a number greater than zero. >
.BASE 432
ADTENOTCR/FAO=0/WARNING-
<RETURN �򲡤������α����򤷤ʤ��ǲ�������>
!<Do not respond by only pressing RETURN. >
.BASE 433
ADTENOTFLD/FAO=1/WARNING-
<����ǥå���������̾�λ���ˤ�����������ߤΥե�����ɤ�̾������Ѥ��Ʋ�������>
!<To specify an index key name, use the name of a field!/you have previously defined. >
.BASE 434
ADTENOTUNQ/FAO=0/WARNING-
<���Υե�����ɤϥ����Ȥ��ƴ��˻��Ѥ���Ƥ��ޤ���>
!<You have already used this field as a key. >
.BASE 435
ADTENUMBIG/FAO=1/WARNING-
<���Υե�����ɤϤ��줬�ޤळ�ȤΤǤ���ǡ����Υ����פ��Ф���Ĺ�����ޤ���!/�����礭���ϣ������Ķ���ƤϤ����ޤ���>
!<This field is too long for the type of data it can contain.!/Its size cannot exceed 18 digits. >
.BASE 436
ADTENUMSML/FAO=1/WARNING-
<�����ι�׷���������Ǥ�����ä��礭����ο������Ϥ��Ʋ�������>
!<The total number of digits is zero.!/Please enter a larger number of digits. >
.BASE 437
ADTENAMBIG/FAO=1/WARNING-
<����̾���� DATATRIEVE ̾�Σ���ʸ�������¤�Ķ���Ƥ��ޤ���!/�̤�̾�������Ϥ��Ʋ�������>
!<This name exceeds the 30-character limit for DATATRIEVE names.!/Please enter another name. >
.BASE 438
ADTEFLDSML/FAO=1/WARNING-
<�쥳���ɤ���Υե�����ɤϥ�����Ĺ����Ȥ뤳�ȤϤǤ��ޤ���!/���Ĥ������������Ʋ�������>
!<A field in a record cannot have a length of zero.!/Please answer with an integer. >
.BASE 439
ADTEINVNAM/FAO=1/WARNING-
<!AC �� DATATRIEVE ��̾���ε�§�˰�ȿ���Ƥ��ޤ���!/������̾�������Ϥ��Ʋ�������>
!<!AC violates the rules for DATATRIEVE names.!/Please enter a valid name. >
.BASE 440
ADTEYESNO/FAO=0/WARNING-
<YES(Y) �ޤ��� NO(N) �������Ʋ�������>
!<Please answer with either YES (Y) or NO (N). >
.BASE 441
ADTFCHAR/FAO=6/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) !SL ʸ�� >
!<!#* !AC !#*(!AC!#*) !SL character!%S >
.BASE 442
ADTFDATE/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) �����դǤ���>
!<!#* !AC !#*(!AC!#*) is a date >
.BASE 443
ADTFGROUP/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) >
!<!#* !AC !#*(!AC!#*) >
.BASE 444
ADTFMONEY/FAO=6/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) �������κ��� !SL �塤���� 2 �塤��ۤǤ���>
!<!#* !AC !#*(!AC!#*) !SL digit!%S left (and 2 digits right) of the decimal, is money >
.BASE 445
ADTFNUMBERS/FAO=7/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) �������κ��� !SL �塤���� !SL ��Ǥ���>
!<!#* !AC !#*(!AC!#*) !SL digit!%S left and !SL digit!%S right of the decimal >
.BASE 446
ADTFPERCENT/FAO=5/INFORMATIONAL-
<!#* !AC !#*(!AC!#*) �ϥѡ�����ȤǤ���>
!<!#* !AC !#*(!AC!#*) is a percent >
.BASE 447
ADTIDEF/FAO=3/INFORMATIONAL-
<!/!/�ɥᥤ��� DATATRIEVE �����!/�ե����� !AD �ˤ���ޤ���!/�쥳����Ĺ�� !SL �Х��ȤǤ���>
!<!/!/The DATATRIEVE definitions for your domain are!/located in file !AD!/The record length is !SL bytes. >
.BASE 448
ADTENOHELP/FAO=0/WARNING-
<���ξ��֤��Ф��� HELP �ƥ����Ȥ϶��뤵��Ƥ��ޤ���>
!<No HELP text has been supplied for this state. >
.BASE 449
ADTENOPROMPT/FAO=0/WARNING-
<���ξ��֤��Ф��ƥץ���ץȡ��ƥ����Ȥ϶��뤵��Ƥ��ޤ���>
!<No prompt text has been supplied for this state. >
.BASE 450
SHOSEMI/FAO=0/INFORMATIONAL-
<    ���ߥ����󤢤�>
!<    Semicolon >
.BASE 451
SHONOSEMI/FAO=0/INFORMATIONAL-
<    ���ߥ�����ʤ�>
!<    No semicolon >
.BASE 452
GUINOWORD/FAO=0/INFORMATIONAL-
<��ɤ�����夬��ޤ���>
!<Can't complete word >
.BASE 453
ADTIBACKUP/FAO=0/INFORMATIONAL-
<ADT �����Ĥξ��֤�Хå����åפ��Ƥ��ޤ���>
!<ADT backing up one state >
.BASE 454
ADTIVIDEO/FAO=0/INFORMATIONAL-
"ADT  ? - �إ��  ! - �ե������  < - �Хå����å�  <PF2> - �����꡼�󡦥إ��"
!" A D T   ? - Help   ! - Fields   < - Back up   <PF2> - Screen Help "
.BASE 455
CURRFAIL/FAO=1/ERROR-
<�ɥᥤ�� !AC ����Υ쥳���ɤ����å���³�Τ���Υ���󥷡�������˼��Ԥ��ޤ�����>
!<Record from source !AC failed to set up currency for set connection. >
!<Record from domain !AC failed to set up currency for set connection. >
.BASE 456
NOTINIMAG/FAO=0/INFORMATIONAL-
<���� DATATRIEVE ��ǽ�� ���Υ��᡼�������줿���ˤ�!/�ޤ���Ƥ��ޤ���Ǥ�����>
!<That DATATRIEVE feature was not included when this image was built. >
.BASE 457
GUINOLINE/FAO=0/INFORMATIONAL-
<�Ԥ�����夬��ޤ���>
!<Can't complete line >
.BASE 458
OLDPLOT/FAO=2/ERROR-
<�ץ��å� !AD �ϸߴ����Τʤ��ե����ޥåȤǵ�������Ƥ��ޤ���!/�������Ф��ƺ�������ʤ���Фʤ�ޤ���>
!<The plot !AD is stored in an incompatible format.  You must extract and re-define it. >
.BASE 459
PLONUMARG/FAO=0/WARNING-
<�����ץ��åȸƤӽФ����Ф��ƶ��뤵�줿�����θĿ������äƤ��ޤ���>
!<Wrong number of arguments supplied to internal plot call. >
.BASE 460
GPRFLOAT/FAO=0/INFORMATIONAL-
<�����ե������>
!<numeric field >
.BASE 461
GPRALPHA/FAO=0/INFORMATIONAL-
<�ѿ����ե������>
!<alphanumeric field >
.BASE 462
GPRDATE/FAO=0/INFORMATIONAL-
<���եե������>
!<date field >
.BASE 463
GPRRETURN/FAO=0/INFORMATIONAL-
"<return>"
!"<return> "
.BASE 464
NOTSYNONYM/FAO=2/ERROR-
<!AD ��Ʊ�����̾���ǤϤ���ޤ���>
!<"!AD" is not the name of a synonym. >
.BASE 465
FORMS/FAO=0/INFORMATIONAL-
<�����ɤ��줿�ե�����: >
!<Loaded forms: >
.BASE 466
NOFORMS/FAO=0/INFORMATIONAL-
<�����ɤ��줿�ե����ब����ޤ���>
!<No loaded forms. >
.BASE 467
FORMINLIB/FAO=4/INFORMATIONAL-
<    �ե����� !AD, !AD ��>
!<    Form !AD in !AD >
.BASE 468
SYNONYMS/FAO=0/INFORMATIONAL-
<  Ʊ����: >
!<Synonyms: >
.BASE 469
SHOWSYN/FAO=4/INFORMATIONAL-
<    !AD  !_for  !AD >
!<    !AD  !_for  !AC >
.BASE 470
RMSKEYUSE/FAO=3/INFORMATIONAL-
<!AC �֡���黻�� RMS �������ե������ !AD ��Ǽ¹Ԥ���Ƥ��ޤ���>
!<Performing !AC boolean on RMS key field !AD >
.BASE 471
CLAIGNORE/FAO=1/WARNING-
<COMPUTED BY ��λ��ѻ��� !AC �᤬̵�뤵��Ƥ��ޤ���>
!<!AC clause is ignored when you use a COMPUTED BY clause. >
.BASE 472
COMFILNES/FAO=0/ERROR-
<���ޥ�ɡ��ե�����Υͥ�������Ķ��Ǥ���>
!<Command file nesting limit exceeded. >
.BASE 473
CLAIGNGRP/FAO=1/WARNING-
<���롼��.�ե����������� !AC �᤬̵�뤵��Ƥ��ޤ���>
!<!AC clause ignored in group field definition. >
.BASE 474
PLOTSAT/FAO=1/INFORMATIONAL-
<!AC ����Υץ��å�: >
!<Plots from !AC: >
.BASE 475
RMSERROR/FAO=2/INFORMATIONAL-
<RMS�ե����� "!AD" ���ѻ��Υ��顼�Ǥ���>
!<Error using RMS file "!AD". >
.BASE 476
RMSCHG/FAO=0/ERROR-
<�ѹ���ǧ����Ƥ��ʤ� RMS �������ե�����ɤ��ͤ������뤳�ȤϤǤ��ޤ���>
!<You cannot modify the value of an RMS key field that doesn't allow changes. >
.BASE 477
RMSDUP/FAO=0/ERROR-
<��ʣ��ǧ����Ƥ��ʤ� RMS �������ե�����ɤ˽�ʣ�ͤ򵭲����뤳�ȤϤǤ��ޤ���>
!<You cannot store a duplicate value in an RMS key field that doesn't allow duplicates. >
.BASE 478
BADSORLEN/FAO=1/ERROR-
<�礭�������ͤޤ���¿�������ͤǥ����Ȥ��褦�Ȥ��ޤ������ʹ�ץ�����: !UW ) >
!<You tried to sort by too large a value or too many values.  (Total size: !UW) >
.BASE 479
NOLISTKEY/FAO=2/ERROR-
<�ե������ "!AD" �� OCCURS �ꥹ�ȤΣ����ʤΤǡ�!/�����Ȥ��ƻ��ѤǤ��ޤ���>
!<Field "!AD" is part of an OCCURS list, so it cannot be used as a key. >
.BASE 480
BADQUOSTR/FAO=0/ERROR-
<����ΰ����䤬����٤��ʤΤˡ�"***END_OF_LINE***" �˽в񤤤ޤ�����>
!<Expected closing quote, encountered "***END_OF_LINE***". >
.BASE 481
NOFMI/FAO=0/ERROR-
<�ե����ॺ���ѥå����������ѤǤ��ޤ���>
!<There is no forms package available. >
.BASE 482
GPRATTOP/FAO=0/INFORMATIONAL-
<AT TOP OF �ޤ��� PRINT >
!<AT TOP OF or PRINT >
.BASE 483
GPRATBOT/FAO=0/INFORMATIONAL-
<AT BOTTOM OF �ޤ��� END-REPORT >
!<AT BOTTOM OF or END-REPORT >
.BASE 484
GPRFLDPAG/FAO=0/INFORMATIONAL-
<�ե������̾�ޤ��� PAGE>
!<Field name or PAGE>  
.BASE 485
GPRPRINT/FAO=0/INFORMATIONAL-
<PRINT >
!<PRINT >
.BASE 486
FMSITT/FAO=0/ERROR-
<����ü���Ǥϥե����ब���ѤǤ��ޤ���>
!<Invalid terminal type for forms. >
.BASE 488
GUINUMBER/FAO=0/INFORMATIONAL-
"<����> "
!"<a number> "
.BASE 489
SHOWEDIT/FAO=0/INFORMATIONAL-
<  �Խ�������: >
!<Edit setting: >
.BASE 490
EDITBACK/FAO=0/INFORMATIONAL-
<    �Խ��ΥХå����åפ򤹤�>
!<    Edit backup >
.BASE 491
EDITNOBACK/FAO=0/INFORMATIONAL-
<    �Խ��ΥХå����åפ򤷤ʤ�>
!<    Edit no backup >
.BASE 492
RMSLKWAIT/FAO=0/INFORMATIONAL-
<���å����줿 RMS �쥳���ɤ��ԤäƤ��ޤ���>
!<Waiting for a locked RMS record... >
.BASE 493
SHOLCKWAT/FAO=0/INFORMATIONAL-
<    ���å��Ԥ�����>
!<    Lock wait >
.BASE 494
SHONOLCKWAT/FAO=0/INFORMATIONAL-
<    ���å��Ԥ����ʤ�>
!<    No lock wait >
.BASE 495
ILLASCIICHAR/FAO=1/WARNING-
<ASCII ʸ�� "!AS" �ϻ��ѤǤ��ޤ���>
!<Illegal ASCII character "!AS". >
.BASE 496
LOTOVEFLO/FAO=0/SEVERE-
<�������顼�ʥ��å����ơ��֥�˻��ѤǤ��륹���åȤ�����ޤ���) >
!<Internal error (no more slots available in lock table). >
.BASE 497
SELRECCLE/FAO=0/INFORMATIONAL-
<�ǥåɥ��å��ˤ�� SELECT �����Ԥ��ޤ�����>
!<SELECT failed due to deadlock. >
.BASE 498
SELRECLOS/FAO=0/INFORMATIONAL-
<�� READY �Τ������򤵤줿�쥳���                                                                                                                                                                                                                                                                           "J� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         � "     x       �ɤθ����˼��Ԥ��ޤ�����>
!<Failed to retrieve a seleted record following a re-ready. >
.BASE 499
NOMORESEL/FAO=1/INFORMATIONAL-
<���쥯����� !AC �ˤϤ�Ϥ����򤵤줿�쥳���ɤ�����ޤ���>
!<Collection !AC will no longer have a selected record. >
.BASE 500
LOSSOFPREC/FAO=0/INFORMATIONAL-
<���ѱ黻������٤������ޤ�����>
!<Loss of precision during arithmetic calculation. >
.BASE 501
REMSELBOO/FAO=0/ERROR-
<�֡���黻�ˤ�� SELECT �ϱ�֥��쥯�����Ǥϥ��ݡ��Ȥ���Ƥ��ޤ���>
!<SELECT with a boolean is not supported for remote collections. >
.BASE 502
GHESET/FAO=0/INFORMATIONAL-
<��ά���ޤ��ϥץ��åȼ�������>
!<Set default dictionary or plot dictionary >
.BASE 503
NODATA/FAO=1/ERROR-
<�ӥ塼����Υե������ "!AD" ���ͤȤ��ƻ��ѤǤ��ޤ���>
!<Field "!AD" from view cannot be used as a value. >
.BASE 504
BADGUICHA/FAO=0/ERROR-
<�����ɡ��⡼�ɤ��Ф������ͥ��ֹ椬����������ޤ���>
!<Invalid channel number for Guide Mode. >
.BASE 505
BADGUITT/FAO=0/ERROR-
<����ü���Ǥϥ����ɡ��⡼�ɤ����ѤǤ��ޤ���>
!<Invalid terminal type for Guide Mode. >
.BASE 506
HELPCONTIN/FAO=0/INFORMATIONAL-
"³������� <return> �򲡤��Ʋ�������"
!"Type <RETURN> to continue. "
.BASE 507
NOFORMFLD/FAO=0/ERROR-
<�ե�����˥ե�����ɤ�¸�ߤ��ޤ���>
!<Field does not exist on form >
.BASE 508
GPRALLVALRET/FAO=0/INFORMATIONAL-
"ALL, �ͤμ��ޤ��� <return>"
!"ALL, value expression or <return> "
.BASE 509
GPRATPRIEND/FAO=0/INFORMATIONAL-
<AT ���椬����ޤ���PRINT �ޤ��� END_REPORT ���Ʋ�������>
!<AT control break, PRINT or END_REPORT >
.BASE 510
GPRBREAK/FAO=0/INFORMATIONAL-
<PAGE, REPORT �ޤ��ϥ����Ȥ��줿�ե������̾>
!<PAGE, REPORT or sorted field name >
.BASE 511
GPRBY/FAO=0/INFORMATIONAL-
<BY �����ȡ��ե�����ɡ��ꥹ��>
!<BY sort field list >
.BASE 512
GPRDECINCFLD/FAO=0/INFORMATIONAL-
<DECREASING, INCREASING �ޤ��ϥե������̾>
!<DECREASING, INCREASING or field name >
.BASE 513
GPRFILNAM/FAO=0/INFORMATIONAL-
<�ե�����̾>
!<file name >
.BASE 514
GPRNUMRET/FAO=0/INFORMATIONAL-
"���͡�<space> �ޤ��� <return> �ǽ�λ "
!"number, end with a <space> or <return> "
.BASE 515
GPROF/FAO=0/INFORMATIONAL-
<OF ����֥졼��>
!<OF control break >
.BASE 516
GPRON/FAO=0/INFORMATIONAL-
"ON �ե�����̾�ޤ��� <return>"
!"ON filename or <return> "
.BASE 517
GPROPCOMRET/FAO=0/INFORMATIONAL-
"+, -, *, /, ����� �ޤ��� <return>"
!"one of +, -, *, /, comma or <return> "
.BASE 518
GPROPRET/FAO=0/INFORMATIONAL-
"+, -, *, / �ޤ��� <return>"
!"one of +, -, *, / or <return> "
.BASE 519
GPROPPARRET/FAO=0/INFORMATIONAL-
"+, -, *, /, ) �ޤ��� <return>"
!"one of +, -, *, /, ) or <return> "
.BASE 520
GPRSETOPT/FAO=0/INFORMATIONAL-
<���ꥪ�ץ����>
!<set option >
.BASE 521
GPRSHOWOPT/FAO=0/INFORMATIONAL-
<ɽ�����ץ����>
!<show option >
.BASE 522
GPRTOPBOT/FAO=0/INFORMATIONAL-
<TOP �ޤ��� BOTTOM OF ����֥졼��>
!<TOP or BOTTOM OF control break >
.BASE 523
GPRVALRET/FAO=0/INFORMATIONAL-
"�ͤμ��ޤ��� <return>"
!"value expression or <return> "
.BASE 524
GPRVALUE/FAO=0/INFORMATIONAL-
<�ͤμ�>
!<value expression >
.BASE 525
GHEAT/FAO=0/INFORMATIONAL-
<�ڡ����ޤ��ϥե�����ɤ��ͤ��Ѥ�ä������ͤ�������ޤ�>
!<Print values when the page or a field value changes >
.BASE 526
GHEBOTTOM/FAO=0/INFORMATIONAL-
<�ڡ����ޤ��ϥե�����ɤ��ͤκǸ���ͤ�������ޤ�>
!<Print values at the end of a page or field value >
.BASE 527
GHEBY/FAO=0/INFORMATIONAL-
<ɬ�פʸ�Ǥ�>
!<Required word in the language >
.BASE 528
GHECDDPATH/FAO=0/INFORMATIONAL-
<�ԥꥪ�ɤǶ��ڤ�줿����̾ (A-Z, U-9, -, $ ) >
!<Dictionary names (A-Z, 0-9, _, $) separated with periods >
.BASE 529
GHECOMMA/FAO=0/INFORMATIONAL-
<���Ǥ���ڤ뤿��Υ����>
!<Comma to separate elements >
.BASE 530
GHEDECR/FAO=0/INFORMATIONAL-
<�ե�����ɤ�߽�˥����Ȥ��ޤ�>
!<Sort field from largest to smallest >
.BASE 531
GHEDICT/FAO=0/INFORMATIONAL-
<���ʤ��ξ�ά��������ѹ����ޤ�>
!<Change the dictionary for your domains and procedures >
.BASE 532
GHEENDREP/FAO=0/INFORMATIONAL-
<��ݡ���ʸ��λ���ޤ�>
!<Finish the report statement >
.BASE 533
GHEEQUAL/FAO=0/INFORMATIONAL-
<������>
!<Equals >
.BASE 534
GHEFILNAM/FAO=0/INFORMATIONAL-
<�����ѥե�����̾�ޤ�������>
!<Filename or device for output >
.BASE 535
GHEFIRST/FAO=0/INFORMATIONAL-
<���쥯�����κǽ�Υ쥳����>
!<First record of the collection >
.BASE 536
GHEINCR/FAO=0/INFORMATIONAL-
<�ե�����ɤ򾺽�˥����Ȥ��ޤ�>
!<Sort field from smallest to largest >
.BASE 537
GHELAST/FAO=0/INFORMATIONAL-
<���쥯�����κǸ�Υ쥳����>
!<Last record of the collection >
.BASE 538
GHELFPAREN/FAO=0/INFORMATIONAL-
<���å��ǰϤޤ줿�ͤμ�>
!<Parenthesized value expression >
.BASE 539
GHELITERAL/FAO=0/INFORMATIONAL-
<������ǰϤޤ줿ʸ����>
!<Quoted string >
.BASE 540
GHENEWLINE/FAO=0/INFORMATIONAL-
<��ݡ��Ȥθ��߹Ԥ�λ���ޤ���>
!<Finish the current line of the report >
.BASE 541
GHENOT/FAO=0/INFORMATIONAL-
<��ӤΥ��󥹤�դˤ��ޤ���>
!<Reverse the sense of the comparison >
.BASE 542
GHENUMBER/FAO=0/INFORMATIONAL-
<����>
!<A number >
.BASE 543
GHEOF/FAO=0/INFORMATIONAL-
<ɬ��ɬ�פǤ�>
!<Required word in the language >
.BASE 544
GHEON/FAO=0/INFORMATIONAL-
<��ݡ��Ƚ��Ϥ�ե�����ޤ������֤�����ޤ�>
!<Send report output to a file or device >
.BASE 545
GHEOPER/FAO=0/INFORMATIONAL-
<���ѱ黻�� : +, -, *, �ޤ��� / >
!<Arithmetic operator:  +, -, * or / >
.BASE 546
GHEPAGE/FAO=0/INFORMATIONAL-
<�ڡ����λϤ� �ޤ��Ͻ�����ͤ�������ޤ�>
!<Print values at the start or end of the page >
.BASE 547
GHEPLOTS/FAO=0/INFORMATIONAL-
<�ץ��åȤΤ���μ�����ѹ����ޤ�>
!<Change the dictionary for your plots >
.BASE 548
GHEQUOTE/FAO=0/INFORMATIONAL-
<Ǥ�դ�ʸ����������ǽ�λ - �����Ѵ����Ϥ��Ǥ��ޤ�>
!<Any character, end with a quote >
.BASE 549
GHEREPORT/FAO=0/INFORMATIONAL-
<�ե����ޥåȲ����줿��ݡ��Ȥ�������ޤ�>
!<Create a formatted report >
.BASE 550
GHEREPPRI/FAO=0/INFORMATIONAL-
<���٤ƤΥ쥳���ɤ��ͤ�������ޤ�>
!<Print values for every record >
.BASE 551
GHEREPREP/FAO=0/INFORMATIONAL-
<��ݡ��ȤλϤ�ޤ��Ͻ�����ͤ�������ޤ�>
!<Print values at the start or end of the report >
.BASE 552
GHERTPAREN/FAO=0/INFORMATIONAL-
<�Ĥ����å�>
!<Closing parenthesis >
.BASE 553
GHESHOCOL/FAO=0/INFORMATIONAL-
<���Ѳ�ǽ�ʥ��쥯������ɽ�����ޤ�>
!<Display available collections >
.BASE 554
GHESHOCUR/FAO=0/INFORMATIONAL-
<���ߤΥ��쥯�����ˤĤ��Ƥξܺ٤�ɽ�����ޤ�>
!<Display details about the current collection >
.BASE 555
GHESHODOM/FAO=0/INFORMATIONAL-
<���Ѳ�ǽ�ʥɥᥤ���ɽ�����ޤ�>
!<Display available domains >
.BASE 556
GHESHOPLO/FAO=0/INFORMATIONAL-
<���Ѳ�ǽ�ʥץ��åȤ�ɽ�����ޤ�>
!<Display available plots >
.BASE 557
GHESHOPRO/FAO=0/INFORMATIONAL-
<���Ѳ�ǽ�ʥץ����������ɽ�����ޤ�>
!<Display available procedures >
.BASE 558
GHESHORDY/FAO=0/INFORMATIONAL-
<��ǥ����ɥᥤ��ˤĤ��Ƥξܺ٤�ɽ�����ޤ�>
!<Display details about readied domains >
.BASE 559
GHESTART/FAO=0/INFORMATIONAL-
<��ʬʸ����ǳ���>
!<Starting with a substring >
.BASE 560
GHETOP/FAO=0/INFORMATIONAL-
<�ڡ����ޤ��ϥե�������ͤκǽ���ͤ�������ޤ�>
!<Print values at the start of a page or field value >
.BASE 561
GPRCDDPATH/FAO=0/INFORMATIONAL-
<����ѥ�̾>
!<dictionary path name >
.BASE 562
GHEAVERAGE/FAO=0/INFORMATIONAL-
<�ե�����ɤ�ʿ��>
!<Average of a field >
.BASE 563
GHECOUNT/FAO=0/INFORMATIONAL-
<�쥳���ɿ�>
!<Number of records >
.BASE 564
GHESTDDEV/FAO=0/INFORMATIONAL-
<�ե�����ɤ�ɸ���к�>
!<Standard deviation of a field >
.BASE 565
GHETOTAL/FAO=0/INFORMATIONAL-
<�ե�����ɤ���>
!<Sum of a field >
.BASE 566
GHERUNNING/FAO=0/INFORMATIONAL-
<�ƥ쥳���ɤ������̤�׻����ޤ�>
!<Compute statistical for each record >
.BASE 567
GPRRUNNING/FAO=0/INFORMATIONAL-
<Ϣ³Ū��������>
!<running statistical >
.BASE 568
GHEMAX/FAO=0/INFORMATIONAL-
<�ե�����ɤκ�����>
!<Maximum value of a field >
.BASE 569
GHEMIN/FAO=0/INFORMATIONAL-
<�ե�����ɤκǾ���>
!<Minimum value of a field >
.BASE 570
GHECOMP/FAO=0/INFORMATIONAL-
"<space> "
!"<space> "
.BASE 571
GHETERM/FAO=0/INFORMATIONAL-
"<return> "
!"<return> "
.BASE 572
GHECHAR/FAO=0/INFORMATIONAL-
<Ǥ�դ�ʸ��>
!"<any character> "
.BASE 573
GPRALL/FAO=0/INFORMATIONAL-
<ALL �ͤμ��ꥹ��>
!<ALL value expression list >
.BASE 574
GPRPLOTARG/FAO=0/INFORMATIONAL-
<!AC >
!<!AC >
.BASE 575
GUIWORKING/FAO=0/INFORMATIONAL-
<[�����Ǥ�...]>
!<[Working...] >
.BASE 576
RDSSIGNON/FAO=2/INFORMATIONAL-
<���ܸ� DEC DATATRIEVE ��⡼�ȡ������С�!/�ץ��ȥ��� !UL.!UL >
!<DEC DATATRIEVE Remote Server!/Protocol !UL.!UL >
.BASE 577
RDSSIGNOFF/FAO=1/INFORMATIONAL-
<���ܸ� DEC DATATRIEVE ��⡼�ȡ������С��ϥ��ƥ����� !XL �ǽ�λ���ޤ���>
!<DEC DATATRIEVE Remote Server terminating with status !XL >
.BASE 578
REMCROSS/FAO=0/ERROR-
<CROSS ��Ʊ��Ρ��ɾ�Υɥᥤ��ǤΤ߻Ȥ��ޤ���>
!<CROSS can only be used on domains with the same node specification.>
.BASE 579
REMNOCR�                                                                                                                                                                                                                                                                           Ɣ[� $      
DTRJA061.C                     �         *    axoSF\SLE_PRECISION.MAR;1                                                                                                W     �                         KGP+     '       ��eLqVC)zd�ز�G�h~Z?V����ɐ���������L7@����6q�����̃�ɗ��"�������n0uRE-)C2K*w<t�����ɭ���,X$������ө��i5M5%ӦgDb\0(#��Pd	-wU�vt/<6L,/EuOu&#�ub<�+ K3�Űdt4́u����,j^T����ͣ���.4P,~w~}X-m5V�����kzT!-0mbc�o띭fALpMAT�X]zv>.;$m5sFfK@mdQ*H[0@j����kCo!7r���ǵ_���Ҋ��늛N�+p�M}-}���@������˰����?
!ᲿJQճ�&<TH����<姮꺹����G`ŴmSkAIGBQQw(����Ù�ۄ�)h���-V?B�J^5�	JyY�oo}jxWp��ޱ�����������(Ҙ��������Ё�� 9'����k?]QGdͅ�ϛ��������
rb+G2��`�
ʛ��^2��]���������ʽ���ƕ������ۥ�������޸wH������><1E/,|6�Ӻ�����x$ED?����WpY��?;��U`�W5<n.w�+<�����^m3G+wS)<'&^�����ע��KHFy������〦���P3DX j'���$S7۰��%@G}|@2IU~_n`aN:#3@�7�˝���֊�㗂�8TohZ<؅�����������Ì��z%V3���Yf#�9r�����ӑL8|	cXm*�8}4o��߈�����ǖ���ĀݕpD?	���rC+cKϑ��iT]|92 m,!#0�&Nƣ��П�a�������쁦����ri|k2C����`"��3��)oj??�򅽻�ۡ">)�1Z{T|fI�XuF��[E<%S����J!^".Zh��� ~.&NIG���������ZLo1tru4^��"�����rL�7b'.}.a	Ro��=[�]F�∗�Wp7z;cBszޑ������������ij^A��Pl{o�����Ӯ�펺d"� �������̈ذܓ���W�������0'it9ENY3>��tf3h0�PF_F�GWa_}@Om~1.tEPr\V��Z!|� ��飧A����H����P�)g9u^3o�ל��.����4K)>���:Yr�n10\��
,]$d]9zpJD6�� qo^(nW��˷_;P[Q:a����۵��������%Ob>se0J	L^aYL ̈́��Ȫ΅�緼��zF����wh�@*�fC���ެ�ׅ��暈������Fj#I1g]d!EZcCJD;ilLde0fPHY.k@{e]�	Д��߽�
���'}���������x�15%�Ë��?-Sb5o$cܳ܂��ek����81�@]ql0yO6+pH<i��4rj,tVt,%Rn`����ȱ��٪�����������M 9�V{74!/a��ª����޼�e3���yIlc�����8��醾ǧ����;L���ߛ���荡�A:*.>gk9{m޼P.�� �< ^?��Ț�����կ�䑼�炸�ojd�/zIsHK	�����큒3��gc��:G(bO��󢵄 ��[
74fG�+	�l�t��`<$�j3���H�7��ΜǴ����r�^�lL��ŕ��WJ_mU8�x(�xD[�,�R�W������sL_f�����¨����芨������̈́ǒ����o3]=b4v��݅�����㱚ՠ鍂ͿÛ.I��v$Ku_I#$d�Б���ȓ�(߾��>TQr8l0-	*����������$O �����%�������b��I������Ã�PW[Hq%��k-W^m#`6<	���ֆ�w~������}{�����m<�Ć�E|3p#eVĠ���������򬭨��u��.\98^k8cL,QGhO�r~��EX;TO��Ϥ�n)ź�ĝ��]EM?׼�������"0r�˔t]p,����>\Xm-9BٙӲ��FMGom�ׁ�����B����ܯ�� A7su WyS^L%?@Z Tw���f�|%>W-O1ѓ��O=7B[):�_i�kY1�����~�����\ b)k�V�ăx3(�����%<P_IY/&UfAQ��J^�1p�9V]UuP@}t[vRD嘤Z�ZZu|<ɗ�ӱ8v)#��{�GBT�Cw|I�����т���kan)q(It����b]0$e73���õ����NQd޿����ث���Tmj@䵐��ݪ����Τ�����������͐��,��	f��nb<$pI&�%pIi0x����U���腞5wKjVi�q��������'U6F!1)[x�����&hf??	[WeVxU|>;,�Ҫ���ѵ���Ԏ����s����6t6=؝��Á����~t�^������ѕ�z䴙J[��1��;
pO1R��U���·Dy��BhmO<gQv6Po�$V;�7|n7zV�����X%+S8m%��J]�נ�џ������ϝ���~)��������.�la]�����ٓ�E���t��nP.kw2"D')~�������$:`����<h}�R<vbZ�IRN���=+��ݸ_qa���1�r�ݠ�+ݧ�����}���Ś�\��tf8#��J$*v���P>��=�榵��楓������ǍԒݞ��͏�O"G����A>_5yDS�/�����#~kEt��������kyrLc 6q���Kf��ѝ�Q���f���3.(Ov@*������(CWH4�ܫ����ݴo'E�ψ��=dypeg!MZ_aa_r~<i6�wPJYdc	m�����S��������l+zYF`�ڏ���ޱǺ����͡����ѭ�Ot"
O\���GVs85`Z���Cy[:JhX. .$U0��`q@)���Ա೹�g 5����Ժ�ꆀq���v��Ƿ������ LrI`U�S%GD;bP*�����b����ɩeP
"��ڝ���h|r�7��榞�ԩ�cRDfW�����������䬫�ˑ���~��J 4z'𾬮����ڸ��5~u0pf��Է��{h!�ƭਗ�	Վ��ӿ�����(����>7<$NA'CTI��߶e<-.00C6Y62$_uM�K;2uPe5���}G��������Ӌ�:)m)&����&v+��M q?f|||�����k�Pv$q- xZ@d�=>lO@mQ}(t,m0x~��(T oa'F���������+IJ/y�3ԉ��	@��.lA�����g��E0 7���ť��ۆ  `�+NStt9t����61ibb|SqI��em4
H'Y8���ܝ��{E*��@\e.��ndmp,@pFgMH6TGR����䜩�����x4E,cOC]�Vv]yrؾ���#sw HSx~�\Ȋ�ԛ�Ք�d@rq]~7AWU���a4)|uNdjiDL��pyM/	f�Þ���匃���!TL|�����ʫih9Q~T][����� EGS;I
AC[!"JfKl:G��� ;���/Ycucf۠������'�:)w=5i,Ѻ���qst&_bYB=O�EW.KE[h5��	Cq.`a��䊟�露�,�鍼����*gL-qc=?(/pXZ�������T ./hAU�����$�Ǌ��}l])r���Y&+8������ඓ��������?If'[g:ww3@G_�끎��^6�r��l'Js)F(×�� B�S�ͅ�VA;	ik�ỹ`q���ҡ�mdQ��NfWf��L�]ɝ���Ҹ��񸤝�v����������చ�ľUzo#q���ڥ�LT?;f?1:bnX�C�[�Ľ��+�/Չ���H|@#=M272�����`uF���WKˑ̞����7t����/n�� flG`b~n���t^%-��Ր�uQzRG�T#%Zl`莲����D~IfN		O	F};"TfSUor��D�����ӵ����������3R|�0F���p����˅6_]�ܡo#d�KExNgivlYKNX$UjGu1���>u.:9d ����՛�䬤:ʯ������},(hű�̺��������6d$xl	����������ñ��~|"��K{F5������ض���k%/yx�﷕�����������1M�����c,�뜀����FL)<h�ʕ�л
4Ej	x^�XgQL5������> H%v%Ɇ�����a4�~ugb;dw<hE���٢>cJ!{
 elH%h(Xj,X3:K��-�����tG񾼅Qe����۬���E3P���ـ������Q�������������Բ��ǅTl\`cey�΄�������B�>L���cZv�鑋A�Y[�z�����Ȱ���~Ud('���������?h{O���Ӊ��χ�ڧƟ���smb3i0>ԑ�_6ϯޱz2��ֱ�δXN=�����;Y����XMW/SO���������XK�����ʽ�σ��������������k2Ր���ƫ��~8�wsar/���ǘJ��p9`1-�9,Hm|ugB�����&͍������c��܈[w3g#�����p"CFH
bJn!?Zr4)����m@�ʢ�}X8Say5CL:HQ����a`o7�SNCk��	4QD'a�䞱���t)Jw�ٓ�s�C_Jd~��aM$�ёqe�K)NO�hd$=���)"G<Uk"U9$x��ء��Ѿ�������A1O3L����-rwreb�鿤׳�����(xz�}���������/�M�����qDz	�#vB]��s���Ɗ��ͱۆ���K�IJg�<������%m!I��nך�;f␇e>����R���bOc1]���$�ٔڳ^�Չ�����m4yN.(PiCb&Yx./Y%T=o}U����������jgK�%����M(BpD�Q֊~������]E��뿄�������,*.���{Stf��:,BgM���혆�څ���[P41f��˶�����/'���_S������d6󴷅xmQdyFP����a0)9{u����}O)��%���O�(��������ԝ���2�����9��ñ������:=1�S`D�5n\YN[���ͼ�蛥��򣋈��Ư�٦����. BrDpM	['��ڬ�ɠ�Ȯõ��qf䃂���iw|*?k��²�OrlECvb/[<��ҙ����_B�����Ut���.J9��
jİ@d��01rUX�HY������ꉩ̱������Л��SeUTsuU{|7&%Zg������ƇԒ���������{4�&���b���r'��{�ޥ%���_F]rQ+n7R�#�̓���EXVHW~r1d6;"lT,����Et[.�!eKYN�ȣ�e|h6pi<)b(^x/��J�����ו��������ī��{'�����)'P_��������Ni0E52U46y\�����{"fDƬ�Ь�)QmzO������ù����AG+vc��kY���$AE8,]jmW�Ά2)z�'E),~b['Gf�����q��QAKK�I����ߒ�����MQ�\sά�����Q8dsh:]2l:�����̙�#|����9�?�����/������6i�������_
(jEyFbM:A|E1����K��})}O�ם��nsA���-@`ϴ�LWe]$ƅ��'�阀L����>����>8f`Ldv!(3	*����~×�����$
v�䊷�L�d&i�by�������FSsGz7]|/x���p�軞�~Ŏl�Ŋ�-�*�|�!K����,w7X?j��y���^���Ų���4t�7��IH���c�^!������ϛ����p� xGUX���Y(uX���������kE��#�#0����XFza5vw;GPo]Ai��������������iv+Lm���[=&{8cH Rj}!������׭���ǈ��ͲȰ���ǹ��26Y.='HS�� \`U�����=W{{|�	`dΕ��˟��Ӱ� Yl
rp����n	{H"B]}TO��杞(����ơ���>��U��c8����ٲ�������p5w,}
;���-{f���㼝ˀ�����TՈ���ײĸ�./x-��)L����������֒ե��ۨ�,.����-����KYuh�ا52ߺ�������ߞ�݌��˅�����
	5M"#Q$S0pu0tpv8�����Z$85�����G�F�{G,/#����BaYC-LoNjus'Mc	7\]��O5�����̋Ȓ�ه����_~	_ ���HCN������rr{��|}9`zuWN,WA&/@b>_�ב�����Q\a_=Ob~Y 0;_7	.:Q.k�WV"-god^Vl>a|��씊��e#Wg���4Hm���ʐ��Ļ��)/!i͠�yrd<	&\������8K+�����|K�������Q/a_z@Sb* OG��B�����i��Ӛ�����:eb|tM���Ŧދ�i����������Z}������ʃx=���؁�~_-+^kw\;e~M䢆�����Ϛ�~,������S0_N݊�������l�I49z��L���R\t?P�azy�ŕ��`f���)4Uע�ܼ��lmB�RmSK+Dm-KJ	^W6�����k}f?84�w��2����(dd֛��ʺ3T�$lH3E5�&�����Q�\��uvcCڅ���ā��i��H������B3�ްϚ�R8��<NN���VE�tƠ��;XɅX�6�XO8i��	/1m:�k�`~�{2��vF;u&��ԓ1���|��ï���ܗ��Ҁ�NT0������xx^"w��VaV���Awk������ȯ1)_����7B&�,����Ұ��ܞ�㜥��v_L`NR~,"�ހ��P @7qmK?g8d `��ߘ��['iG^:��򶖺�=0w$:+OCC.Dn)�������@Fv��{ɔ����r���ᒑ��-,PF��������7��@����>{wP{<`2.Qw2u���9�盢l��Ͼw�x(t�ݞ�����������������������G5#ZIaCuIVA>I9���g4+Cl`���ȸ�d_zIp|dpj�� 3b#yTb������ȸ ���C{1sP%dG��xW G���Ts~Ll	x"�͜���mfB~)����٣������\--V5��Ss*��ɯ�̾���ާ��@�=Eey_+}＿87:L��ޑ�ع������߆ޗ��ƚ�����Jv%dG%7�0����f[:��k�tR+������ƍ��h5U3J!*qjzs���+kU)e}v	[��ς����ǧ���̴���ݳ���������#.';d&8bӗ���k[ڊa1V�$Ik�ˁ��y_2Rt**����d��o(=xf

�1,kI|I7�����ٝ���M����ݦ���Ǯ���(R�a2S���������­����ʽ�����!.؍���뺋�����ƈr|!:>�N6Mf;,Θ���Ͱ������-W@rl4����ő����ԇ���ܱ8x�������p:^TZ0@8&l3
ō+�7OpQr	?^ƫ�����'�������������`6<2Zp����Ej:Y���Ű&nL2��==e!Ad]`�`Y���
������*���̆䝢��σ�[R�ȷ���3C9T���B5N]Ŝ�������f"(!4��Iq��˷��HVEtuZ���705	OYO���������빇���4BTP:���䪦�Ʈ��v����zZ
OFz��h*h`zs{����ʱ��������Ϩߗ²�������h����`\��������󖨙f���q24`��%PŤ��X��G֢&�^5ue@��螏�*|/��'��wF0o��:^rJxe"kXv���!���I);�z1so��ishxm�|A9 �f� 
́*�޴�*�!� �l� �.������1X�L�|�]lx|LNZ+��8E��uO`ï�^h-SB" CSDZ��߷:V<S$nmLz��W2����Vg¼��چ�G5��˳������`8	yf����1qP��Z#3���)OG+<��ϯ�^O3��j gw-2qm0WVaL�����U~N$Yul:���������������ĄCz{Ww����QŠ��ȭ�����������tEԝ�����ˬ�՗���滛����RrIn���Ż�G���������Ҙ��u bi�el�ߛ����p��椯��g+qPO^A K��yg7u3G-.:&lz'OTTOM_CORR .EQ. 0) THEN - 		WRITE (6, *)'There is no valid coefficient   	1of correlation'  		p                                                                                                                                                           cψM $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         � "     �       OSS/FAO=0/ERROR-
<���α�֥Ρ��ɤ� CROSS �򥵥ݡ��Ȥ��Ƥ��ޤ���>
!<This remote node does not support CROSS. >
.BASE 580
SOURCE/FAO=0/INFORMATIONAL-
<��ǥ���������: >
!<Ready sources: >
.BASE 581
NOFLDS/FAO=0/ERROR-
<�ե�����ɤξ���Ϥ���ޤ��� >
!<No field information available >
.BASE 582
RDBRB/FAO=3/INFORMATIONAL-
<RDB �ǡ����١��� !AD on !AS; >
!<RDB database !AD on !AS; >
.BASE 583
NOALIAS/FAO=0/ERROR-
<�ǡ����١�������̾�� READY ���뤳�ȤϤǤ��ޤ���>
!<You can not READY a database with an alias. >
.BASE 585
CANRDYSRC/FAO=0/ERROR-
<COMMIT, ROLLBACK �ޤ��� FINISH ��Ԥ��ޤǥ������ϥ�ǥ�����ޤ���>
!<Source cannot be readied unless a COMMIT, ROLLBACK or FINISH is issued. >
.BASE 586
DICNOTRED/FAO=0/ERROR-
<������������뤳�ȤϤǤ��ޤ���>
!<Dictionaries may not be redefined. >
.BASE 587
FILNOTRED/FAO=0/ERROR-
<�ե�������������뤳�ȤϤǤ��ޤ���>
!<Files may not be redefined. >
.BASE 588
REDEFINE/FAO=0/WARNING-
<�������������Ǥ�����ˤ���ޤ��� - �������������ޤ�����>
!<Element to be redefined not found in dictionary - new element defined. >
.BASE 589
NOTRELNAM/FAO=4/ERROR-
<"!AD" �ϥǡ����١��� "!AD" ����������졼�����̾�ǤϤ���ޤ���>
!<"!AD" is not a valid relation name for database "!AD". >
.BASE 590
TRAPURCOL/FAO=2/WARNING-
<������ Rdb �ȥ�󥶥�����󤬳��Ϥ���ޤ���; ���쥯����� !AD �ϼ�ưŪ�˲�������ޤ�����>
!<New Rdb transaction started; collection !AD automatically released. >
.BASE 594
BADFLDTYP/FAO=2/WARNING-
<�ե������ "!AD" �ϥ��ݡ��Ȥ���Ƥ��ʤ��ǡ������ʤΤ�̵�뤵��ޤ�����>
!<Field "!AD" ignored because of unsupported datatype. >
.BASE 596
SNAPTOSHARE/FAO=0/WARNING-
<���� READY SNAPSHOT ����Ƥ��뤤���Ĥ��Υ쥳���ɡ��������ϡ�SHARED READ�ǥ�����������ޤ���>
!<Some record sources currently READYed SNAPSHOT will now be accessed SHARED READ. >
.BASE 597
RDYSNPTOSHR/FAO=0/INFORMATIONAL-
<���ߤ� DBMS ���뤤�� RMS �쥳���ɡ��������� READY SNAPSHOT (READ)�⡼�ɤϡ�READY SHARED READ�⡼�ɤ��ѹ�����ޤ�����>
!<READY SNAPSHOT (READ) mode has been changed to READY SHARED READ mode for the current DBMS or RMS record source. >
.BASE 598
COMROLLCOMP/FAO=0/INFORMATIONAL-
<COMMIT / ROLLBACK ����λ���ޤ����� >
!<COMMIT / ROLLBACK completed. >
.BASE 599
RDBCOMMITSTA/FAO=0/WARNING-
<COMMIT ��ޤ�ʸ���ǽ�� COMMIT �Ƕ�����λ����ޤ�����>
!<Statement containing COMMIT aborted after first COMMIT. >
.BASE 600
RDBCOMPOUND/FAO=0/ERROR-
<RDB �ե�����ɤ򻲾Ȥ���ʣ��ʸ�ˤ� COMMIT ��ޤळ�Ȥ��Ǥ��ޤ���>
!<Compound statements referencing RDB fields may not contain COMMITs. >
.BASE 601
RDBREQSYNC/FAO=0/ERROR-
<�ǡ������Ф�����ä��׵�Τ���ˡ�RDB �ˤ�ꥹ�ơ��ȥ��Ȥ�������λ����ޤ�����>
!<Statement aborted by RDB due to an invalid request for data. >
.BASE 602
RECEXCMAX/FAO=0/ERROR-
<�쥳����Ĺ�������ͤ�Ķ���ޤ����� >
!<Record size exceeds maximum. >
.BASE 603
ROLPURCOL/FAO=2/WARNING-
<ROLLBACK ���¹Ԥ���ޤ���; ���쥯����� !AD �ϼ�ưŪ�˲�������ޤ�����>
!<ROLLBACK executed; collection !AD automatically released. >
.BASE 604
PURKEEPBAD/FAO=0/ERROR-
<PURGE ��¹Ԥ��뤿��� KEEP ���ͤϡ����ʾ�Ǥʤ���Фʤ�ޤ���>
!<The KEEP value for PURGE must be greater than zero. >
.BASE 605
PURELMNOT/FAO=1/WARNING-
<�������� "!AS" �ΤɤΥ��֥������Ȥ�ѡ�������ޤ���>
!<No objects purged for dictionary element "!AS". >
.BASE 606
PURDICNOT/FAO=0/WARNING-
<���ߤξ�ά���μ���ΤɤΥ��֥������Ȥ�ѡ�������ޤ���>
!<No objects purged for the current default dictionary. >
.BASE 607
PURELMSEM/FAO=1/ERROR-
<���֥������� "!AS" ������С�������ѡ������뤳�ȤϽ���ޤ���>
!<You cannot purge a specific version of object "!AS". >
.BASE 608
PURNOTFND/FAO=1/WARNING-
<�������� "!AS" �����Ĥ���ޤ���>
!<Dictionary element "!AS" not found. >
.BASE 609
PURNOTDTR/FAO=1/WARNING-
<���� "!AS" �ϥɥᥤ��,�쥳����,�ץ�������,�ơ��֥�Τɤ�Ǥ⤢��ޤ���>
!<Element "!AS" is not a Domain, Record, Procedure, or Table. >
.BASE 610
DELNOPRIV/FAO=1/WARNING-
<���֥������� "!AS" ��ä����¤�����ޤ���>
!<No privilege to delete object "!AS". >
.BASE 611
SHOCOLREL/FAO=1/INFORMATIONAL-
<��졼�����: !AC >
!<    Relation: !AC >
.BASE 612
SHOCOLREC/FAO=1/INFORMATIONAL-
<  �쥳����: !AC >
!<    Record: !AC >
.BASE 613
RDBNOREL/FAO=2/ERROR-
<�ǡ����١��� "!AD" �ϲ����졼��������äƤ��ʤ��Τ� READY ����ޤ���>
!<Database "!AD" cannot be readied since it contains no relations. >
.BASE 614
BADSEGEDIT/FAO=2/WARNING-
<ʬ��ʸ����ե������ "!AD" �� "T" �Խ�ʸ���������ʤ���Фʤ�ʤ��Τ�/��ά�� T(COLUMNS_PAGE) �Ȥ��ޤ���>
!<Segmented string field "!AD" must have "T" edit string, defaulting to T(COLUMNS_PAGE). >
.BASE 615
NOSEGDEFAULT/FAO=2/WARNING-
<ʬ��ʸ����ե������ "!AD" �Ͼ�ά�ͤ���Ƥʤ��Τ�̵�뤷�ޤ���>
!<Segmented string field "!AD" cannot have a default value, default value will be ignored. >
.BASE 616                                                                
NOSEGMISSING/FAO=2/WARNING-
<ʬ��ʸ����ե������ "!AD" �϶��ͤ���Ƥʤ��Τ�̵�뤷�ޤ���>
!<Segmented string field "!AD" cannot have a missing value, missing value will be ignored. >
.BASE 617
SEGWRITERROR/FAO=2/WARNING-
<ʬ��ʸ����ե������ "!AD" ���񤭽Ф��ޤ���>
!<Error writing to segmented string field "!AD". >
.BASE 618
SEGCANCERROR/FAO=2/WARNING-
<�ե������ "!AD" �Τ���˺�ä�ʬ��ʸ���󤬥���󥻥����ޤ���>
!<Error canceling segmented string created for field "!AD". >
.BASE 619
SEGCLOSERROR/FAO=2/WARNING-
<�ե������ "!AD" ��ʬ��ʸ���󤬥�����������ޤ���>
!<Error closing segmented string for field "!AD". >
.BASE 620
SEGREADERROR/FAO=2/WARNING-
<�ե������ "!AD" ��ʬ��ʸ�����ɤ�ޤ���>
!<Error reading segmented string from field "!AD". >
.BASE 621
SEGCREAERROR/FAO=2/WARNING-
<�ե������ "!AD" �Τ����ʬ��ʸ���󤬥��顼�ΰٺ��ޤ���>
!<Error creating segmented string for output to field "!AD". >
.BASE 622
SEGOPENERROR/FAO=2/WARNING-
<�ե������ "!AD" �Τ����ʬ��ʸ���󤬥��顼�ΰ٥����ץ����ޤ���>
!<Error opening segmented string from field "!AD". >
.BASE 623
NOTSEGMENT/FAO=2/SEVERE-
<"!AD" ��ʬ��ʸ����ե�����ɤǤϤ���ޤ���>
!<"!AD" is not a segmented string field. >
.BASE 624
NOSEGASS/FAO=2/WARNING-
<ʬ��ʸ����ե������ "!AD" �ؤγ�����Ƥ���ȿ�ʤΤ�̵�뤷�ޤ�����>
!<Ignoring illegal assignment to or from segmented string field "!AD". >
.BASE 625
GUITOOMNY/FAO=0/ERROR-
<���ߤΥɥᥤ��Υե�����ɤ�¿������ GUIDE MODE ��³�����ޤ���>
!<The current domain has too many fields for GUIDE MODE to continue. >
.BASE 626
ILLSEGOPR/FAO=2/ERROR-
<ʸ�˰�ȿ�����뤿��,ʬ��ʸ����ե������ "!AD" ��Ŭ�ѽ���ޤ���>
!<Statement is illegal for segmented string field "!AD". >
.BASE 627
ASSERROR/FAO=0/WARNING-
<���������˥��顼��ȯ�����ޤ�����>
!<Error occurred during assignment. >
.BASE 628
BADASSIGN/FAO=2/WARNING-
<���������Υ��顼�Τ���, �ѿ� "!AD" ���ͤ��������ʤ����⤷��ޤ���>
!<Variable "!AD" may contain an incorrect value due to error during assignment. >
.BASE 629
BADMODSTO/FAO=2/WARNING-
<STORE ���� MODIFY �ǤΥ��顼�Τ���, �ե������ "!AD" ���ͤ��������ʤ����⤷��ޤ���>
!<Field "!AD" may contain an incorrect value due to error during STORE or MODIFY. >
.BASE 630
RDBERRFIN/FAO=0/WARNING-
<Rdb �ȥ�󥶥�����󤬳��Ͻ���ʤ��Τ�, ��졼����ʥ�ɥᥤ��򤹤٤ƽ�λ�����ޤ���>
!<Unable to start a relational database transaction, finishing all relational domains. >
.BASE 633
CHGSNAPAC/FAO=2/WARNING-
<SNAPSHOT WRITE ���������⡼�ɤǤϤ���ޤ���READY "!AD" SNAPSHOT READ ��¹���Ǥ���>
!<SNAPSHOT WRITE is not a valid access mode.  Readying "!AD" SNAPSHOT READ. >
.BASE 634
BADCONSIST/FAO=0/ERROR-
<Ʊ��ǡ����١������Ф��� CONSISTENCY �� CONCURRENCY ��Ʊ���˻������ޤ���>
!<It is illegal to have CONSISTENCY and CONCURRENCY specified for the same database. >
.BASE 635
NOCONSIST/FAO=3/WARNING-
<"!AC" �� Rdb �Ǥʤ������� "!AD" �ˤϻ������ޤ���̵�뤷�ޤ���>
!<Ignoring "!AC" specified for non relational database source "!AD". >
.BASE 636
DIFFERENTDB/FAO=2/WARNING-
<Ʊ��̾���Ǵ��� READY ����Ƥ��륽����������Τ� "!AD" �� READY ����ޤ���>
!<"!AD" cannot be readied with the same name as a currently readied source. >
.BASE 637
NOSNAPSDBM/FAO=1/INFORMATIONAL-
<"!AC" �� SNAPSHOT �⡼�ɤǥ�����������ޤ��󡣥⡼�ɤ� SHARED READ �ˤ��ޤ���>
!<"!AC" cannot be accessed in SNAPSHOT mode.  Changing mode to SHARED READ. >
.BASE 638
NOTRDYSNAP/FAO=1/WARNING-
<DBMS �������� SNAPSHOT ������ʤ��Τ� "!AC" �� READY ����ޤ���>
!<DBMS source does not permit snapshots.  "!AC" cannot be readied SNAPSHOT. >
.BASE 639
NOSNAPCOM/FAO=0/WARNING-
<DBMS �������� SNAPSHOT �⡼�ɤ� READY ����Ƥ���֤� COMMIT ����ޤ���>
!<A COMMIT cannot be done while there is a DBMS source readied in SNAPSHOT mode. >
.BASE 640
INVEDITOR/FAO=1/ERROR-
<!AS �����ܸ� DEC DATATRIEVE �ǻ�                                                                                                                                                                                                                                                                           �*U $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         )  "     �       Ȥ��륨�ǥ����ǤϤ���ޤ���>
!<!AS is not a valid editor within DEC DATATRIEVE. >
.BASE 641
NOCODTRAN/FAO=0/ERROR-
<�ơ��֥�ˤϥ����ɤȤ����������Ȥ��ʤ���Фʤ�ޤ���>
!<Table must include a code/translation pair. >
.BASE 642
RECNOTOPT/FAO=0/WARNING-
<�쥳���ɤ��������ޤ���, ��Ŭ������ޤ���>
!<Record will be defined, but will not be optimized. >
.BASE 643
PLOHDRLNG/FAO=0/ERROR-
<�ץ��åȤΥ����ȥ�Ԥ�49ʸ���ʲ��Ǥ���>
!<A title line for a plot cannot exceed 49 characters. >
.BASE 644
RDYNOTOPT/FAO=1/WARNING-
<�쥳���� "!AC" �κ�Ŭ��������ޤ���Ǥ�����������³���ޤ���>
!<Use of optimization for record "!AC" failed.  Processing will continue. >
.BASE 645
DROPOPT/FAO=2/WARNING-
<�쥳���� "!AD" ��������饭����� "OPTIMIZE" �򳰤��ޤ�����>
!<Keyword "OPTIMIZE" removed from definition of record "!AD". >
.BASE 646
RECNOTCUR/FAO=1/WARNING-
<�쥳���� "!AC" �Υե����ޥåȤ��Ť���ΤǤ�����!/������³���ޤ���, ��Ŭ���ΰ٤˺�������������ɤ��Ǥ��礦��>
!<Record "!AC" uses old record format.  Processing will!/continue, but for optimization you must redefine record. >
.BASE 647
STACKSIZE/FAO=1/INFORMATIONAL-
<���ܸ�DATATRIEVE �Υ����å��� !UL. >
!<DEC DATATRIEVE stack size is !UL. >
.BASE 648
EXTBADPARAM/FAO=0/ERROR-
<�����ꥹ�Ȥˤϥ��֥������ȥѥ�̾�ȥ��֥������ȥ����פ�ξ���ϻ������ޤ���>
!<Argument list cannot contain both the object path name and the object type. >
.BASE 649
BADRDYMODE/FAO=2/WARNING-
<DTR$READY_MODE ���� "!AD" �������������������ץ����ǤϤ���ޤ���>
!<The value of DTR$READY_MODE, "!AD", is not a valid access option. >
.BASE 650
RDYMODIGN/FAO=0/WARNING-
<�嵭�Υ��顼�� DTR$READY_MODE �˸��Ĥ��ä��Τǥ����������ץ����ˤϾ�ά�ͤ�Ȥ��ޤ���>
!<!/The preceding error occurred while translating DTR$READY_MODE.!/Therefore, the DATATRIEVE default access option will be used. >
.BASE 651
PLOTOOBIG/FAO=0/ERROR-
<�ץ��åȥ������������ͤ�ۤ��ޤ�����>
!<Maximum plot size exceeded. >
.BASE 652
BADEDIT/FAO=0/ERROR-
<���顼�ΰ�, ���ǥ���������ޤ���>
!<Error encountered when entering editor. >
.BASE 653
REINITCDD/FAO=0/INFORMATIONAL-
<CDD ��ƽ�������ޤ���>
!<Reinitializing CDD >
.BASE 654
CDDCORRUPT/FAO=0/ERROR-
<CDD ���ƽ��������ޤ���DATATRIEVE ���öȴ�����Ƴ����Ƥ��� CDD �˥����������ƤߤƲ�������>
!<CDD could not be reinitialized.  Exit and reinvoke DATATRIEVE!/before attempting to access the CDD. >
.BASE 655
ILLSEGEDIT/FAO=2/ERROR-
<ʬ��ʸ����ե������ "!AD" �ˤ� "T"(text) �Խ�ʸ���󤬤ʤ���Фʤ�ޤ���>
!<Segmented string field "!AD" must have a "T" (text) edit string. >
.BASE 656
BADKEYTBL/FAO=0/ERROR-
<�����ơ��֥�ID��̵���Ǥ���>
!<Invalid key table ID. >
.BASE 657
BADKEYBRD/FAO=0/ERROR-
<�����ܡ���ID��̵���Ǥ���>
!<Invalid key board ID. >
.BASE 658
KEYSTATE/FAO=1/INFORMATIONAL-
<!AS �����륭���ѥå������: >
!<!AS state keypad definitions: >
.BASE 659
KEYDEF/FAO=1/INFORMATIONAL-
<    !AS = "!AS" >
!<    !AS = "!AS" >
.BASE 660
KEYATRBTS/FAO=6/INFORMATIONAL-
<        (!ACecho,!ACterminate,!ACerase,!AClock!AC!AS) >
!<        (!ACecho,!ACterminate,!ACerase,!AClock!AC!AS) >
.BASE 661
INVKEYMOD/FAO=1/WARNING-
<"!AS" �����إ����ѥåɥ⡼�ɤȤ���̵���Ǥ���>
!<"!AS" is not a valid alternate keypad mode. >
.BASE 662
FDLERROR/FAO=1/ERROR-
<FDL �ե����� "!AD" ������˥��顼��ȯ�����ޤ�����>
!<Error using FDL file "!AD". >
.BASE 663
FDLSTMNT/FAO=2/ERROR-
<FDL �ե����� "!AD" ������˥��顼��ȯ�����ޤ��������λ���ʸ�� !UL �Ǥ���>
!<Error using FDL file "!AD"; statement !UL. >
.BASE 664
FDLOPEN/FAO=1/ERROR-
<FDL �ե����� "!AD" �Υ����ץ���˥��顼��ȯ�����ޤ�����>
!<Error opening FDL file "!AD". >
.BASE 665
FDLBADSIZ/FAO=2/ERROR-
<�쥳���ɥե����뤬̷�⤷�Ƥ��ޤ�������ͤ�: !UW, FDL �ե������: !UW >
!<Incompatible record size.  Defined: !UW FDL File: !UW >
.BASE 666
EXCCMPSTK/FAO=1/ERROR-
<��٥� !UL �ǥ���ѥ��饹���å���٥��ۤ��ޤ�����>
!<Exceeded compiler stack level at level !UL. >
.BASE 667
UNSHOWDB/FAO=2/WARNING-
<!AD Rdb ��ɽ������ޤ���>
!<Unable to show !AD relational database. >
.BASE 668
APPKEYPAD/FAO=0/INFORMATIONAL-
<    ���ץꥱ������� �����ѥå� �⡼��>
!<    Application keypad mode >
.BASE 669
NUMKEYPAD/FAO=0/INFORMATIONAL-
<    �˥塼���å� �����ѥå� �⡼��>
!<    Numeric keypad mode >
.BASE 670
BADSIGN/FAO=1/WARNING-
<ʸ���� "!AS" ���̵���ʥ����󤬤���ޤ���>
!<Invalid sign or signs in string "!AS". >
.BASE 671
FUNCAPTIVE/FAO=0/ERROR-
<��°��������ȤǤϴؿ���¹Խ���ޤ���>
!<Function cannot be executed from a captive account. >
.BASE 672
INFBADIDX/FAO=0/ERROR-
<����ե����᡼����󥤥�ǥå������³���ۤ��ޤ�����>
!<Information index is out of range. >
.BASE 673
RELDBSHOW/FAO=2/INFORMATIONAL-
<Relational database !AD : >
!<Relational database !AD : >
.BASE 674
MAXSCRLIN/FAO=0/ERROR-
<����������꡼�����κ���Կ���ۤ��ޤ�����>
!<Maximum lines for scrolling region exceeded. >
.BASE 675
INVBLKSIZ/FAO=1/SEVERE-
<�������顼(̵���ʥ�����: !XL �Υ֥��å�����Ф����Ȥ��ޤ�����) >
!<Internal error (attempted to get a block of invalid size: !XL). >
.BASE 676
RELINK/FAO=0/ERROR-
<DATATRIEVE ���᡼����ƺ������뤫 DATATRIEVE ��ƥ��󥹥ȡ��뤷�Ʋ�������>
!<Please relink your DATATRIEVE image or reinstall DATATRIEVE >
.BASE 677
MAXEXTEXC/FAO=0/ERROR-
<EXTRACT ���� EDIT �ΥХåե��������κ����ͤ�ۤ��ޤ�����>
!<Maximum size for extract or edit buffer exceeded. >
.BASE 678
CDDONLY/FAO=1/WARNING-
<CDO ����Ρ��� "!AS" ���Ф��Ƥ������ϹԤ��ޤ���>
!<You cannot perform this operatin on CDO dictionary node, "!AS". >
.BASE 679
INVBUF/FAO=0/SEVERE-
<�������顼�� CDD/Plus �᥿�ǡ������Хåե�������������ޤ��󡣡�>
!<Internal error (invalid CDD/Plus metadata buffer). >
.BASE 680
DEFLIST/FAO=1/INFORMATIONAL-
<                       = !AC >
!<                       = !AC >
.BASE 681
NOGOCDO/FAO=1/ERROR-
<DATATRIEVE !AC �� CDO �ǥ��쥯�ȥ�˳�Ǽ�Ǥ��ޤ���>
!<A DATATRIEVE !AC cannot be stored in a CDO directory. >
.BASE 682
DROPRELAT/FAO=2/WARNING-
<RELATIONSHIPS �� DMU ���֥������Ȥˤϻ��ѤǤ��ޤ���/!���ν����Ҥϥɥᥤ�� "!AD" ����������ޤ�����>
!<RELATIONSHIPS cannot be applied to DMU objects.!/The RELATIONSHIPS qualifier has been removed from domain "!AD". >
.BASE 683
RELONLYCDO/FAO=0/WARNING-
<RELATIONSHIPS �� CDO ���֥������Ȥˤ������ѤǤ��ޤ���>
!<RELATIONSHIPS will be applied only to CDO objects. >
.BASE 684
PASTTOP/FAO=0/ERROR-
<�ѥ�̾�Υϥ��ե�¿�����ޤ���>
!<You used too many hyphens in a path name. >
.BASE 685
NOWINDOWS/FAO=0/ERROR-
<������ɥ������󥿥ե������Ǥϻ��ѤǤ��ޤ���>
!<The DATATRIEVE window interface is not active. >
.BASE 686
NOCOMMAND/FAO=0/INFORMATIONAL-
<���ޥ�����ϥ��ơ��ȥ��Ȥ��������������Ǥ��Ƥ��ޤ���>
!<DATATRIEVE is not ready to process a command or statement. >
.BASE 687
INVOPTION/FAO=0/ERROR-
<DTR$WINDOWS ������������ʥ��ץ���󤬻��ꤵ��ޤ�����>
!<An invalid option has been specified for the DTR$WINDOWS call. >
.BASE 688
NOSMG/FAO=0/ERROR-
<SMG �����ܡ��ɡ����󥿥ե������Ǥϻ��ѤǤ��ޤ���>
!<The DATATRIEVE SMG keyboard interface is not active. >
.BASE 689
PRINTDAB/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 690
NOSMGINFO/FAO=0/INFORMATIONAL-
<SMG �����ܡ��ɡ����󥿥ե������Ǥϻ��ѤǤ��ޤ���>
!<The DATATRIEVE SMG keyboard interface is not active. >
.BASE 691
MESSAGE/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 692
OUTPUT/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 693
FATAL/FAO=0/SEVERE-
<������ɥ������󥿥ե������ˤ����Ʋ����Բ�ǽ�ʥ��顼��ȯ�����ޤ�����������λ���ޤ���>
!<Unrecoverable window interface error, performing image exit. >
.BASE 694
BADFETCH/FAO=0/ERROR-
<������ɥ������󥿥ե������ˤ����� DRM�ǡ����١������饪�֥������Ȥ���Ф��ޤ���Ǥ�����>
!<Unable to fetch an object from the window interface DRM database. >
.BASE 695
CSERROR/FAO=0/ERROR-
<������ɥ������󥿥ե������ˤ�����ʣ��ʸ������Ѵ��˼��Ԥ��ޤ�����>
!<Windows interface compound string conversion error. >
.BASE 696
XLIBERR/FAO=0/INFORMATIONAL-
<XLIB ���饨�顼��������ޤ�����>
!<Error received from XLIB : >
.BASE 697
INVOKEFILCOM/FAO=1/INFORMATIONAL-
<@!AS; >
!<@!AS; >
.BASE 698
CLOSEFILCOM/FAO=0/INFORMATIONAL-
<CLOSE; >
!<CLOSE; >
.BASE 699
EXITCOM/FAO=0/INFORMATIONAL-
<EXIT; >
!<EXIT; >
.BASE 700
SHOWREADYCOM/FAO=0/INFORMATIONAL-
<SHOW READY; >
!<SHOW READY; >
.BASE 701
SHOWFIELDSCM/FAO=0/INFORMATIONAL-
<SHOW FIELDS; >
!<SHOW FIELDS; >
.BASE 702
SHOWCURRENT/FAO=0/INFORMATIONAL-
<SHOW CURRENT; >
!<SHOW CURRENT; >
.BASE 703
SHOWCOLLECTS/FAO=0/INFORMATIONAL-
<SHOW COLLECTIONS; >
!<SHOW COLLECTIONS; >
.BASE 704
SHOWVARIABLE/FAO=0/INFORMATIONAL-
<SHOW VARIABLES; >
!<SHOW VARIABLES; >
.BASE 705
SHOWSETSCOM/FAO=0/INFORMATIONAL-
<SHOW SETS; >
!<SHOW SETS; >
.BASE 706
OPENLOGFILE/FAO=1/INFORMATIONAL-
<OPEN !AS; >
!<OPEN !AS; >
.BASE 707
SHOWFIELDSFR/FAO=1/INFORMATIONAL-
<SHOW F�                                                                                                                                                                                                                                                                           ��(� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         �� "     �       IELDS FOR !AS; >
!<SHOW FIELDS FOR !AS; >
.BASE 708
SHOWNAMEDCOM/FAO=1/INFORMATIONAL-
<SHOW !AS; >
!<SHOW !AS; >
.BASE 709
SETCOLSPAGE/FAO=1/INFORMATIONAL-
<SET COLUMNS_PAGE = !AS; >
!<SET COLUMNS_PAGE = !AS; >
.BASE 710
READYSOURCE/FAO=5/INFORMATIONAL-
<READY !AS !AS !AS !AS !AS; >
!<READY !AS !AS !AS !AS !AS; >
.BASE 711
EXTRACTOBJ/FAO=2/INFORMATIONAL-
<EXTRACT !AS !AS; >
!<EXTRACT !AS !AS; >
.BASE 712
SETABORTCOM/FAO=0/INFORMATIONAL-
<SET ABORT; >
!<SET ABORT; >
.BASE 713
SETNOABORT/FAO=0/INFORMATIONAL-
<SET NO ABORT; >
!<SET NO ABORT; >
.BASE 714
SETLOCKWAIT/FAO=0/INFORMATIONAL-
<SET LOCK_WAIT; >
!<SET LOCK_WAIT; >
.BASE 715
SETNOLOCKWAI/FAO=0/INFORMATIONAL-
<SET NO LOCK_WAIT; >
!<SET NO LOCK_WAIT; >
.BASE 716
SETPROMPTCOM/FAO=0/INFORMATIONAL-
<SET PROMPT; >
!<SET PROMPT; >
.BASE 717
SETNOPROMPT/FAO=0/INFORMATIONAL-
<SET NO PROMPT; >
!<SET NO PROMPT; >
.BASE 718
SETSEARCH/FAO=0/INFORMATIONAL-
<SET SEARCH; >
!<SET SEARCH; >
.BASE 719
SETNOSEARCH/FAO=0/INFORMATIONAL-
<SET NO SEARCH; >
!<SET NO SEARCH; >
.BASE 720
SETSEMICOLON/FAO=0/INFORMATIONAL-
<SET SEMICOLON; >
!<SET SEMICOLON; >
.BASE 721
SETNOSEMICOL/FAO=0/INFORMATIONAL-
<SET NO SEMICOLON; >
!<SET NO SEMICOLON; >
.BASE 722
SETVERIFY/FAO=0/INFORMATIONAL-
<SET VERIFY; >
!<SET VERIFY; >
.BASE 723
SETNOVERIFY/FAO=0/INFORMATIONAL-
<SET NO VERIFY; >
!<SET NO VERIFY; >
.BASE 724
RSNAPSHOT/FAO=0/INFORMATIONAL-
<SNAPSHOT >
!<SNAPSHOT >
.BASE 725
RPROTECTED/FAO=0/INFORMATIONAL-
<PROTECTED >
!<PROTECTED >
.BASE 726
RSHARED/FAO=0/INFORMATIONAL-
<SHARED >
!<SHARED >
.BASE 727
REXCLUSIVE/FAO=0/INFORMATIONAL-
<EXCLUSIVE >
!<EXCLUSIVE >
.BASE 728
RREAD/FAO=0/INFORMATIONAL-
<READ >
!<READ >
.BASE 729
RMODIFY/FAO=0/INFORMATIONAL-
<MODIFY >
!<MODIFY >
.BASE 730
RWRITE/FAO=0/INFORMATIONAL-
<WRITE >
!<WRITE >
.BASE 731
REXTEND/FAO=0/INFORMATIONAL-
<EXTEND >
!<EXTEND >
.BASE 732
RCONSISTENCY/FAO=0/INFORMATIONAL-
<CONSISTENCY >
!<CONSISTENCY >
.BASE 733
RCONCURRENCY/FAO=0/INFORMATIONAL-
<CONCURRENCY >
!<CONCURRENCY >
.BASE 734
NOGODMU/FAO=0/ERROR-
<DMU ����˳�Ǽ�Ǥ��ʤ�°��������˴ޤޤ�Ƥ��ޤ���>
!<Definition contains attributes that cannot be stored in DMU dictionary. >
.BASE 735
NOSELECTMEM/FAO=0/ERROR-
<CUT,COPY,PASTE ������ɬ�פʥ��꤬�����Ǥ��ޤ���>
!<Unable to allocate memory for CUT or COPY or PASTE selection. >
.BASE 736
NOSELECTION/FAO=0/ERROR-
<���ߤΥ��쥯����󤬤���ޤ���>
!<No current selection exists. >
.BASE 737
CLIPFAIL/FAO=0/ERROR-
<DECwindows ����åץܡ��ɤΥ��������˼��Ԥ��ޤ�����>
!<Failure to access DECwindows clipboard. >
.BASE 738
CLIPTRUNCATE/FAO=0/ERROR-
<DECwindows ����åס��ܡ��ɤΥǡ������ڤ�ΤƤ��ޤ�����>
!<DECwindows clipboard data truncated. >
.BASE 739
CLIPLOCKED/FAO=0/ERROR-
<DECwindows ����åס��ܡ��ɤ����å�����Ƥ��ޤ����⤦���ٻ�Ʋ�������>
!<The DECwindows clipboard is locked, please try again later. >
.BASE 740
CLIPNODATA/FAO=0/ERROR-
<DECwindows ����åס��ܡ��ɤˤϸ��ߥǡ���������ޤ���>
!<No data is currently contained on the DECwindows clipboard. >
.BASE 741
PASTELENGTH/FAO=0/ERROR-
<Ž���դ�������ܤϥƥ����ȡ��ե�����ɤ�Ĺ����ۤ��Ƥ��ޤ���>
!<Item to be pasted is too large for text field. >
.BASE 742
RELMSG/FAO=8/WARNING-
<"!AD" �������� "!AD" �˴ط��Ť����Ƥ��ޤ���!/������ !AD ����ƥ��ƥ� "!AD"�Ǥ���>
!<"!AD" is related to an invalid "!AD", triggered by !/!AD entity "!AD". >
.BASE 743
POSMSG/FAO=6/WARNING-
<"!AD" ���������ʤ����⤷��ޤ���!/������ !AD ����ƥ��ƥ� "!AD" �Ǥ���>
!<"!AD" is possibly invalid, triggered by !/!AD entity "!AD". >
.BASE 744
INVMSG/FAO=6/WARNING-
<"!AD" ������������ޤ���!/������ !AD ����ƥ��ƥ� "!AD" �Ǥ���>
!<"!AD" is invalid, triggered by !/!AD entity "!AD". >
.BASE 745
CHILDMSG/FAO=6/WARNING-
<"!AD" �ϥ�졼����󥷥åפ��Ѥ���줿����ƥ��ƥ��˴ط��Ť����Ƥ��ޤ���!/������ !AD ����ƥ��ƥ� "!AD" �Ǥ���>
!<"!AD" is related to an entity whose relationships have !/changed, triggered by !AD entity "!AD". >
.BASE 746
NOMESSAGES/FAO=2/INFORMATIONAL-
<"!AD" �ˤϥ�å������Ϥ���ޤ���>
!<"!AD" has no messages. >
.BASE 747
NEWVERMSG/FAO=6/WARNING-
<"!AD" �Ͽ������С���������ĥ���ƥ��ƥ�����Ѥ��Ƥ��ޤ���!/������ !AD ����ƥ��ƥ� "!AD" �Ǥ���>
!<"!AD" uses an entity which has new versions, triggered by !/!AD entity "!AD". >
.BASE 748
UNKMSG/FAO=6/WARNING-
<"!AD" �������ʥ����פΥ�å���������äƤ��ޤ���!/������ !AD ����ƥ��ƥ� "!AD" �Ǥ���>
!<"!AD" has a message of unknown type, triggered by !/!AD entity "!AD". >
.BASE 749
MSGPROB/FAO=2/WARNING-
<"!AD" �� CDD/Plus ��å���������Ф����˥��顼��ȯ�����ޤ�����>
!<An error occured while fetching CDD/Plus messages for "!AD". >
.BASE 750
SHOWALL/FAO=0/INFORMATIONAL-
<SHOW ALL; >
!<SHOW ALL; >
.BASE 751
SHOWDICT/FAO=0/INFORMATIONAL-
<SHOW DICTIONARIES; >
!<SHOW DICTIONARIES; >
.BASE 752
SHOWDOMAINS/FAO=0/INFORMATIONAL-
<SHOW DOMAINS; >
!<SHOW DOMAINS; >
.BASE 753
SHOWPROCED/FAO=0/INFORMATIONAL-
<SHOW PROCEDURES; >
!<SHOW PROCEDURES; >
.BASE 754
SHOWRECS/FAO=0/INFORMATIONAL-
<SHOW RECORDS; >
!<SHOW RECORDS; >
.BASE 755
SHOWTABS/FAO=0/INFORMATIONAL-
<SHOW TABLES; >
!<SHOW TABLES; >
.BASE 756
SHOWDICTION/FAO=0/INFORMATIONAL-
<SHOW DICTIONARY; >
!<SHOW DICTIONARY; >
.BASE 757
UNKNOWN/FAO=0/INFORMATIONAL-
<UNKNOWN >
!<UNKNOWN >
.BASE 758
INVOKEADT/FAO=0/INFORMATIONAL-
<ADT; >
!<ADT; >
.BASE 759
SETGUIDE/FAO=0/INFORMATIONAL-
<SET GUIDE; >
!<SET GUIDE; >
.BASE 760
BADRELNAM/FAO=2/WARNING-
<��졼�����̾������������ޤ���"!AD" �����Ѥ���Ĥ���ڤ�ΤƤޤ�����>
!<Invalid relation name.  "!AD" will be used and the remainder dropped. >
.BASE 761
EDITCOMMAND/FAO=0/INFORMATIONAL-
<EDIT; >
!<EDIT; >
.BASE 762
NODECTERM/FAO=0/ERROR-
<DECterm ������ɥ��������Ǥ��ޤ���DTR$LIBRARY:REMOTE_DECTERM.COM ��ư���Ʋ�������>
!<Unable to create a DECterm window - execute DTR$LIBRARY:REMOTE_DECTERM.COM for remote displays. >
.BASE 763
NOTFLDNAM/FAO=1/ERROR-
<"!AS" �ϥե������̾�ǤϤ���ޤ���>
!<"!AS" is not a field name. >
.BASE 764
NOTALLREL/FAO=0/WARNING-
<�ӥ塼�����Ȥ��Ƥ���ɥᥤ�� RELATIONSHIPS ����ʤ����������Ƥ���Τǡ�!/���٤ƤΥӥ塼�Υ�졼�����Ϻ�������ޤ���>
!<Some domains referenced by view were defined without RELATIONSHIPS.  Therefore!/not all view relationships will be formed. >
.BASE 765
OVFMETABF/FAO=2/SEVERE-
<�������顼�ʥ᥿�ǡ������Хåե� �����С��ե��� length = !UL, used = !UL ��>
!<Internal error (Meta data buffer overflow, length = !UL, used = !UL). >
.BASE 766
DISMISSWORK/FAO=0/INFORMATIONAL-
<�ʹ���ܥå����򽪤�餻�ޤ���>
!<Dismiss work in progress box. >
.BASE 767
MEMSTUCK/FAO=0/SEVERE-
<�������顼�ʲ��ۥ��꤬�����Ǥ��ޤ���Ǥ�������>
!<Internal error (virtual memory could not be released). >
.BASE 768
NEWDOMVER/FAO=2/INFORMATIONAL-
<DEFINE FILE �ѥ�᡼�����������ͤ�̷�⤷�Ƥ��ޤ���!/"!AD" �ο������С�����󤬺�������ޤ�����>
!<DEFINE FILE parameters conflict with previous values.  Therefore!/a new version of "!AD" has been created. >
.BASE 769
INVACLATT/FAO=2/ERROR-
<"!AD" �������ʣ��ã�°��������ޤ�����>
!<Invalid ACL attribute found for "!AD". >
.BASE 770
CDDNOTFOUND/FAO=1/ERROR-
<���� "!AS" �ϼ���ˤ���ޤ���> 
!<Element "!AS" cannot be found in the dictionary. >
.BASE 771
CDONOTFOUND/FAO=1/ERROR-
<���� "!AS" �� CDO ����ˤ���ޤ���>
!<Element "!AS" is not in the CDO dictionary. >
.BASE 772
PROTNOTSUPP/FAO=1/ERROR-
<���� "!AS" ���ݸ���������ˤ� CDO ����Ѥ��Ʋ�������>
!<Use CDO to define protection for element "!AS". >
.BASE 773
NOPASSWORD/FAO=1/WARNING-
<CDO ���� "!AS" �Υѥ���ɤ�̵�뤵��ޤ�����>
!<Password ignored for CDO element "!AS". >
.BASE 774
NOTERMINAL/FAO=3/WARNING-
<CDO ���� "!+!AS" �� TERMINAL ���ץ���� "!-!-!AD" ��̵�뤵��ޤ�����>
!<TERMINAL option "!AD" ignored for CDO element "!AS". >
.BASE 775
NOBANISH/FAO=1/WARNING-
<CDO ���� "!AS" �� BANISH ���ץ�����̵�뤵��ޤ�����>
!<BANISH option ignored for CDO element "!AS". >
.BASE 776
CDOIDENT/FAO=2/INFORMATIONAL-
<  !UB:!AS >
!<  !UB:!AS >
.BASE 777
CDOACCESS/FAO=1/INFORMATIONAL-
<        !AS >
!<        !AS >
.BASE 778
DTRACCESS/FAO=2/INFORMATIONAL-
<        Grant - !AS, Deny - !AS >
!<        Grant - !AS, Deny - !AS >
.BASE 779
NOGLOBDELETE/FAO=0/WARNING-
<CDO ����� G (GLOBAL_DELETE) ���ץ�����̵�뤵��ޤ�����>
!<G (GLOBAL_DELETE) protection option ignored for CDO dictionary. >
.BASE 780
NOPASSTHRU/FAO=0/WARNING-
<CDO ����� P (PASS_THRU) ���ץ�����̵�뤵��ޤ�����>
!<P (PASS_THRU) protection option ignored for CDO dictionary. >
.BASE 781
NODMUEXTEND/FAO=0/WARNING-
<CDO ����� X (EXTEND) ���ץ�����̵�뤵��ޤ�����>
!<X (EXTEND) protection option ignored for CDO dictionary. >
.BASE 782
NOFORWARD/FAO=0/WARNING-
<CDO ����� F (FORWARD) ���ץ�����̵�뤵��ޤ�����>
!<F (FORWARD) protection option ignored for CDO dictionary. >
.BASE 783
NOHI�                                                                                                                                                                                                                                                                           �;d $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         s "     �       STORY/FAO=0/WARNING-
<CDO ����� H (HISTORY) ���ץ�����̵�뤵��ޤ�����>
!<H (HISTORY) protection option ignored for CDO dictionary. >
.BASE 784
INVDIRPROT/FAO=2/ERROR-
<���� "!AD" �������� ACL ���ߤĤ���ޤ�����>
!<Invalid ACL found for directory "!AD". >
.BASE 785
DIFFNODE/FAO=4/ERROR-
<��֥ɥᥤ�󡦥��󥫡��ΥΡ���̾ "!AD" �ϥΡ��ɻ��� "!AD" �Ȱ��פ��ޤ���>
!<Node name in remote domain anchor, "!AD", does not!/match name in node specification, "!AD". >
.BASE 786
NOGRANTDENY/FAO=1/ERROR-
<"!AS" �ˤ� GRANT�� ���� DENY�����ꤷ�ʤ���Фʤ�ޤ���>
!<GRANT or DENY must be specified for "!AS". >
.BASE 787
BADSEQUENCE/FAO=0/WARNING-
<ACL ���� ACE ������ޤ���>
!<No ACL exists at sequence number position. >
.BASE 788
IGCMDARGS/FAO=2/WARNING-
<"!AD" �� CDD$DATABASE �ɥᥤ��Ǥ����ե�����ϥɥᥤ���°���˽��ä��������ޤ�����!/DEFINE FILE ���ޥ�ɤΥ���������Ȥ�̵�뤵��ޤ�����>
!<"!AD" is a CDD$DATABASE domain.  File defined according to!/domain attributes.  DEFINE FILE command line arguments ignored. >
.BASE 789
SHOWPROT/FAO=2/INFORMATIONAL-
<!AD >
!<!AD >
.BASE 790
SHOPRIVRN/FAO=0/INFORMATIONAL-
< R (CDO READ)           - READ �Ѥ� READY�Ǥ��ޤ���SHOW,EDIT,EXTRACT����Ѥ��Ʋ�������>
!< R (CDO READ)           - may ready for READ, use SHOW, EDIT and EXTRACT >
.BASE 791
SHOPRIVWN/FAO=0/INFORMATIONAL-
< W (CDO WRITE)          - READ,WRITE,MODIFY ���� EXTEND �Ѥ� READY �Ǥ��ޤ���>
!< W (CDO WRITE)          - may ready for READ, WRITE, MODIFY or EXTEND >
.BASE 792
SHOPRIVMN/FAO=0/INFORMATIONAL-
< M (CDO MODIFY)         - READ, MODIFY �Ѥ� READY �Ǥ��ޤ���>
!< M (CDO MODIFY)         - may ready for READ, MODIFY >
.BASE 793
SHOPRIVEN/FAO=0/INFORMATIONAL-
< E (CDO EXTEND+WRITE)   - EXTEND �Ѥ� READY �Ǥ��ޤ���>
!< E (CDO EXTEND+WRITE)   - may ready for EXTEND >
.BASE 794
SHOPRIVCN/FAO=0/INFORMATIONAL-
< C (CDO CONTROL)        - DEFINEP, SHOWP, DELETEP ���ޥ�ɤ��Ȥ��ޤ��� >
!< C (CDO CONTROL)        - may issue DEFINEP, SHOWP, DELETEP commands >
.BASE 795
SHOPRIVDN/FAO=0/INFORMATIONAL-
< D (CDO DELETE)         - �ǥ��쥯�ȥꡦ���֥����������϶��Υǥ��쥯�ȥ�����Ǥ��ޤ���>
!< D (CDO DELETE)         - may delete a dictionary object or empty directory >
.BASE 796
SHOWPRIVSN/FAO=0/INFORMATIONAL-
< S (CDO SHOW)           - ���񥪥֥������Ȥ򸫤���ɤ�ˤ��Ȥ��Ǥ��ޤ���>
!< S (CDO SHOW)           - may see dictionary object >
.BASE 797
SHOPRIVUN/FAO=0/INFORMATIONAL-
< U (CDO DEFINE+CHANGE)  - ���񥪥֥������Ȥ�������Ϲ����Ǥ��ޤ���>
!< U (CDO DEFINE+CHANGE)  - may define or update dictionary object >
.BASE 798
RMSDBSHOW/FAO=8/INFORMATIONAL-
<CDD$DATABASE !AD �ϰʲ�����Ѥ��Ƥ��ޤ�!/    RMS_DATABASE : !AD !/    �쥳���� : !AD !/    �ե����� : !AD >
!<CDD$DATABASE !AD uses!/    RMS_DATABASE : !AD !/    RECORD : !AD !/    FILE : !AD >
.BASE 799
INVOKEPROC/FAO=1/INFORMATIONAL-
<:!AS; >
!<:!AS; >
.BASE 800
READYDEFAULT/FAO=1/INFORMATIONAL-
<READY !AS; >
!<READY !AS; >
.BASE 801
SETCDDDICT/FAO=1/INFORMATIONAL-
<SET DICTIONARY !AS; >
!<SET DICTIONARY !AS; >
.BASE 802
BADELEMENTID/FAO=0/ERROR-
<���򤵤줿 CDD���Ǥ򥢥������Ǥ��ޤ���>
!<Unable to access selected CDD element. >
.BASE 803
NONAVIGATOR/FAO=0/ERROR-
<CDD �ʥӥ�������ư�Ǥ��ޤ���Ǥ�����>
!<Unable to invoke the CDD navigator. >
.BASE 804
CANTSPAWN/FAO=0/WARNING-
<���֥ץ�������ư�Ǥ��ޤ���Ǥ�����> 
!<Subprocess could not be spawned. >
.BASE 805
EDITOBJECT/FAO=1/INFORMATIONAL-
<EDIT !AS; >
!<EDIT !AS; >
.BASE 806
NOCDOCMD/FAO=0/ERROR-
<CDO ���ޥ�ɤ�ͽ�������Τˡ������˽в񤤤ޤ�����>
!<Expected CDO command, encountered end of line. >
.BASE 807
DECWOUTPUT/FAO=1/INFORMATIONAL-
<!AS >
!<!AS >
.BASE 808
HLPACTIVE/FAO=0/INFORMATIONAL-
<�إ�ס�������ɥ��ϴ��˻��Ѥ���Ƥ��ޤ���>
!<Help window already active. >
.BASE 809
DMUFROMFLD/FAO=1/ERROR-
<DMU �쥳���� "!AS" �� FROM �ե�����ɤǤϻ��ȤǤ��ޤ���>
!<DMU record "!AS" may not be referenced in FROM field. >
.BASE 810
DEFSCROLLIN/FAO=1/WARNING-
<���ꤵ�줿����������Կ�ʬ�Υ��꤬�����Ǥ��ޤ���Ǥ����������� !UL �Ȥ��ޤ���>
!<Unable to allocate scroll lines specified, defaulting to !UL. >
.BASE 811
BADFLDEXP/FAO=2/WARNING-
<�����������ʤ��Τǡ��ե������ "!AD" ��̵�뤵��ޤ�����>
!<Field "!AD" ignored because of invalid expression. >
.BASE 812
IGNVALEXP/FAO=2/WARNING-
<�ե������ "!AD" �� VALID IF ��μ����������ʤ��Τǡ�̵�뤵��ޤ�����>
!<Invalid VALID IF expression in field "!AD" will be ignored. >
.BASE 813
SENDMAINXY/FAO=0/INFORMATIONAL-
<DAB �ˤ���ᥤ�󡦥�����ɥ��Σء��٥ݥ���������ʤ�����>
!<Request to place main window X,Y position in DAB. >
.BASE 814
BADHLPINIT/FAO=0/WARNING-
<�إ�ס�������ɥ�������������꤬ȯ�����ޤ�����>
!<Problem encountered initializing HELP window. >
.BASE 815
COLSPAGE/FAO=0/INFORMATIONAL-
<COLUMNS_PAGE �����꤬�Ѥ��ޤ�����>
!<The columns_page setting has changed. >
.BASE 816
SETFORMSCOM/FAO=0/INFORMATIONAL-
<SET FORM; >
.BASE 817
SETNOFORMS/FAO=0/INFORMATIONAL-
<SET NO FORM; >
.BASE 818
ILLOPERVIEW/FAO=1/ERROR-
<��졼����ʥ롦�ӥ塼�������� "!AC" ���Ф��������ʥ��ڥ졼����󤬹Ԥʤ��ޤ����� >
!<Illegal operation for relational view source "!AC". >
.BASE 819
BADSOURCE/FAO=2/ERROR-
<���֥������� "!AD" �������� DATATRIEVE �������ǤϤ���ޤ���>
!<Object "!AD" is not a valid DATATRIEVE source. >
!
! --- Following mesages are added from V6.1.
!
.BASE 820
BADLOGNAM/FAO=1/WARNING-
<����̾ !AS �ν�����˥��顼��ȯ�����ޤ�����>
!<Error found while processing logical name !AS >
.BASE 821
RESATTNAM/FAO=2/ERROR-
<"!AD" �Ϥ��Ǥ�ͽ�󤵤�Ƥ���°��̾�Ǥ���>
!<"!AD" is a reserved attribute name >
.BASE 822
DROPUSING/FAO=2/WARNING-
<FORM IS...USING �� DMU �ɥᥤ��ˤ�Ŭ�ѤǤ��ޤ���!/USING ��� "!AD" �ɥᥤ�󤫤�������ޤ�����>
!<FORM IS...USING cannot be applied to DMU domains.!/The USING clause has been removed from domain "!AD" >
.BASE 823
DFLOADFORM/FAO=0/ERROR-
<DECfroms �Υե�����Υ����ɤ˼��Ԥ��ޤ�����>
!<Failure attempting to load a DECforms form. >
.BASE 824
DFNOLAYOUT/FAO=0/ERROR-
<DECforms �Υե�����ˡ����Υ����ߥʥ뷿�����졢���̥������Ȱ��פ����Τ�����ޤ���>
!<No layouts in the DECforms form conform to this terminal type,language, and display size. >
.BASE 825
DFNOLICENSE/FAO=0/ERROR-
<DECforms �Υե����ࡦ�ޥ͡����㡼�Ϥ���ޤ�����ͭ���ʥ饤���󥹤�����ޤ���>
!<DECforms form manager is present, but no DECforms software license is active >
.BASE 826
DFOPENFORM/FAO=2/ERROR-
<DECforms �Υե����ࡦ�ե�����Υ����ץ�ǥ��顼��ȯ�����ޤ�����>
!<Error opening DECforms form file !AD . >
.BASE 827
DFREADFORM/FAO=2/ERROR-
<DECforms �Υե����ࡦ�ե�������ɤ߹��ߤ˼��Ԥ��ޤ�����>
!<Error reading DECforms form file !AD . >
.BASE 828
DFBADRECCNT/FAO=0/ERROR-
<DATATRIEVE �Υ쥳���ɿ����ե����ࡦ�쥳���ɡ��ꥹ�ȤȰ��פ��ޤ���>
!<The number of DATATRIEVE records does not match the number of records in the form record list. >
.BASE 829
DFNORECORD/FAO=0/ERROR-
<����оݤΥե����ࡦ�쥳���ɤ� DECfroms �Υե�������˸��Ĥ���ޤ���>
!<A form record specified in the operation is unknown in the DECforms form. >
.BASE 830
DFDBLATTEMPT/FAO=0/INFORMATIONAL-
<DECforms �� FMS, TDMS �Τɤ�⤬���ꤵ�줿�ե�����򥢥������Ǥ��ޤ���>
!<Both DECforms and FMS/TDMS are unable to access the specified form >
.BASE 831
DFNOTNOW/FAO=0/ERROR-
<���� DECforms ��Ȥ��ޤ���>
!<A DECforms form cannot be used now. >
.BASE 832
NOFORMAT/FAO=0/ERROR-
<�֤�������쥳����������ɤ߹���ޤ���>
!<Unable to load exchange record definition. >
.BASE 833
NOWITHF/FAO=0/ERROR-
<SET NO FORM �����ꤵ��Ƥ������ WITH_FORM ʸ�ϻȤ��ޤ���>
!<Is not possible to use the WITH_FORM statement when a SET NO FORM is active >
.BASE 834
NOFTREE/FAO=0/ERROR-
<�ե�������ڤ�������뤳�Ȥ��Ǥ��ޤ���>
!<Unable to build the Field Tree >
.BASE 835
NOEXCHANGE/FAO=0/ERROR-
<�������ˤ� EXCHANGE �쥳���ɤ�ɬ�פǤ���>
!<An EXCHANGE record is needed to perform this operation >
.BASE 836
CDANOTINST/FAO=0/ERROR-
<��ݡ��Ȥκ�������ߤ��ޤ�(CDA ���᡼���˥��������Ǥ��ޤ���)��>
!<Aborting report execution- cannot access CDA image >
.BASE 837
NODELETE/FAO=1/WARNING-
<��֥ե����� !AS �����Ǥ��ޤ���>
!<Could not delete temporary file !AS >
.BASE 838
CDAERROR/FAO=0/INFORMATIONAL-
<CDA toolkit �Υ��顼�Τ����ݡ��Ȥκ�������ߤ��ޤ���>
!<Aborting report execution due to CDA toolkit error. >
.BASE 839
FMTNEEDON/FAO=1/ERROR-
<!AS �ե����ޥåȤΥ�ݡ��Ȥκ����ˤϡ��ե�����̾����Τ���� ON �礬ɬ�פǤ���>
!<Execution of a report in !AS format requires an ON clause specifying the file name. >
.BASE 840
SYNTAX_ERR/FAO=2/ERROR-
<"!AD" �ն�Ǥ�ʸˡ���顼�Ǥ���>
!<Syntax error at or near "!AD" . >
.BASE 841
TOOMANYID/FAO=0/SEVERE-
<�������顼�Ǥ�(���̻Ҥγ�Ǽ�ΰ��ۤ��ޤ���)��>
!<Internal error (exceeded identifier array size). >
.BASE 842
CDONOT�                                                                                                                                                                                                                                                                           �8� $      
DTRJA061.C                     �  
  [C]DTRMSGS_JA.MSG;1                                                                                                            �     �                         ��      �       RANS/FAO=0/ERROR-
<���� CDO �������� DATATRIEVE �Υ�����������Ǥ��ޤ���>
!<Unable to build Datatrieve source text from this CDO definition. >
.BASE 843
DFBADRECLEN/FAO=0/ERROR-
<DATATRIEVE �Υ쥳����Ĺ�ȥե����ࡦ�쥳���ɤ�Ĺ�������פ��ޤ���>
!<The length of the DATATRIEVE record and the form record do not match. >
.BASE 844
COLTOOBIG/FAO=0/ERROR-
<���쥯�������Υ쥳���ɿ���¿�����ޤ���>
!<Too many records for this collection >
.BASE 845
DTYPEUNS/FAO=1/ERROR-
<!AC �ǡ������Ϥ��Υץ�åȥե�����Ǥϥ��ݡ��Ȥ���Ƥ��ޤ���>
!<!AC data type is not supported on this platform. >
.BASE 846
FLDTYPCNV/FAO=2/WARNING-
<�ե������ "!AD" �ϥ��ݡ��Ȥ���Ƥ��ʤ��ǡ������ʤΤ� !AC ���Ѵ����ޤ�����>
!<Field "!AD" converted to !AC because of unsupported datatype. >
.BASE 847
LITTYPCNV/FAO=1/WARNING-
<���ݡ��Ȥ���Ƥ��ʤ��ǡ������Υ�ƥ��� !AC ���Ѵ����ޤ�����>
!<A literal was converted to !AC because of unsupported data type. >
.BASE 848
NOTRDB/FAO=2/ERROR-
<"!AD" �ϥ�졼�����ǤϤ���ޤ���>
!<"!AD" is not a relation. >
.BASE 849
INVWDTHRANGE/FAO=2/ERROR-
<!UL �� WIDTH ���ͤ� !AC ���ѻ極����������ޤ���>
!<WIDTH value of !UL not in range for !AC paper orientation. >
.BASE 850
NOTELEST/FAO=2/ERROR-
<���ܥե�����ɤǤʤ��ե������ !AD �����״ؿ��ǻ��ѤǤ��ޤ���>
!<Non-elementary field !AD cannot be used in a statistical function. >
!+DTR_I18N V6.1
!	Expanded message for I18N.
.BASE 851
BADCSNAM/FAO=4/WARNING-
<���ݡ��Ȥ���Ƥ��ʤ�ʸ�����åȤ����ꤵ��ޤ������ǥե�����ͤ���Ѥ��ޤ���>
!<Unspported character set was specified.  Default value will be used.>
!
!-DTR_I18N V6.1
.END
                                                                                                                                                                                                                                                                                                                                                                                                                                                         �               * [C]DTRTEXT.MAR;1 +  , �   . $    / s P   4 Q   $   " �                   - 
    0  � 1    2   3      K � P   W   O #    5   6  �����  7 @�*�*�  8          9          G s P  H �� J �              " .TITLE DTRTEXT VAX Datatrieve TextF ;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ;		   DATATRIEVE TEXT  ; Q ;       Copyright (c) Digital Equipment Corporation 1981, 1982, 1983, 1984, 1986.  ;			   All rights reserved.  ; H ; This software is furnished under a license and may be used and  copiedH ; only  in  accordance  with  the  terms  of  such  license and with theH ; inclusion of the above copyright notice.  This software or  any  otherH ; copies  thereof may not be provided or otherwise made available to anyH ; other person.  No title to and ownership of  the  software  is  hereby ; transferred. ; H ; The information in this software is subject to change  without  noticeH ; and  should  not  be  construed  as  a commitment by Digital Equipment ; Corporation. ; H ; Digital assumes no responsibility for the use or  reliability  of  its9 ; software on equipment which is not supplied by Digital.  ;  ;    FUNCTIONAL DESCRIPTION  ;  ; & ;	Definitions of user changeable text. ;  ;    VERSION NUMBER  ;	5 F ;---------------------------------------------------------------------   .PSECT TEXT,NOWRT,SHR,PIC,2   .LIBRARY /DTR$LIBRARY:DTRTXTLIB/  : ;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ;  ;	$ D T R $ T E X T _ I N I T  ; % ; Assigns a value to the text "name". . ; Defines a buffer for an array to store text. ; : ;--------------------------------------------------------- $DTR$TEXT_INIT  : ;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ;  ;	$ D T R $ T E X T _ E N T R Y  ; > ; Makes an entry of text into the buffer and creates an offsetC ; into the buffer. These text strings can be modified by end users.  ; : ;---------------------------------------------------------  $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_APR,-
 		^\April\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_AUG,- 		^\August\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_DEC,- 		^\December\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_FEB,- 		^\February\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_FRI,- 		^\Friday\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_JAN,- 		^\January\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_JUL,-	 		^\July\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_JUN,-	 		^\June\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_MAR,-
 		^\March\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_MAY,- 		^\May\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_MON,- 		^\Monday\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_NOV,- 		^\November\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_NOW,- 		^\NOW\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_OCT,- 		^\October\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_PIC,- 		^\DD-MMM-YYYY?" "\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_SAT,- 		^\Saturday\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_SEP,- 		^\September\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_SUN,- 		^\Sunday\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_THU,- 		^\Thursday\ & $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_TODAY,-
 		^\TODAY\) $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_TOMORROW,-  		^\TOMORROW\ $ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_TUE,- 		^\Tuesday\$ $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_WED,- 		^\Wednesday\' $DTR$TEXT_ENTRY DTR$$K_TXT_DAT_YESTER,-  		^\YESTERDAY\' $DTR$TEXT_ENTRY DTR$$K_TXT_KW_CONCURR,-  		^\CONCURRENCY\' $DTR$TEXT_ENTRY DTR$$K_TXT_KW_CONSIST,-  		^\CONSISTENCY\& $DTR$TEXT_ENTRY DTR$$K_TXT_KW_INIVEC,- 		^\INIT_VECTOR\' $DTR$TEXT_ENTRY DTR$$K_TXT_KW_KEYDEFS,-  		^\KEYDEFS\& $DTR$TEXT_ENTRY DTR$$K_TXT_KW_KEYPAD,- 		^\APPLICATION_KEYPAD\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_KW_OPTIMIZE,- 		^\OPTIMIZE\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_KW_RELATION,- 		^\RELATIONSHIPS\& $DTR$TEXT_ENTRY DTR$$K_TXT_KW_REPLOT,- 		^\REPLOT_LAST\$ $DTR$TEXT_ENTRY DTR$$K_TXT_KW_SEMI,- 		^\;\) $DTR$TEXT_ENTRY DTR$$K_TXT_MSC_FLOATPIC,-  		^\-9.9999E+99\) $DTR$TEXT_ENTRY DTR$$K_TXT_MSC_STDEVPIC,-  		^\ZZZZ9.9999\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_MSC_RDBNULL,-	 		^\NULL\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ALLOCRUL,-  		^\allocation rule\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ARGLIST,- 		^\argument list\# $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_AS,-  		^\AS\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_BOOEXP,-  		^\Boolean expression\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_COLLNAME,-  		^\collection name\& $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_COLON,-! 		^\a colon to separate elements\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_COLPAGE,- 		^\COLUMNS_PAGE\ & $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_COMMA,-! 		^\a comma to separate elements\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_CONSTANT,-  		^\constant\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DATATYPE,-  		^\DATA TYPE\% $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DATE,- 	 		^\DATE\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DATORNUM,-  		^\DATE or NUMBER\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DECLARE,-# 		^\variable name, PORT or SYNONYM\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DEFINOBJ,-  		^\DEFINE type\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DEFINPSU,-  		^\DEFINEP option\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DEFINSUB,-  		^\DEFINE subcommand\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DFNPOPT,- 		^\define privilege option\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DFNPSEQ,-' 		^\protection element sequence number\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DICTPATH,-  		^\dictionary path name\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DOMNAME,- 		^\domain name\$ $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_END,- 		^\END\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ENDCASE,-( 		^\END_CHOICE, ELSE clause, or boolean\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ENDCOM,-  		^\end of command\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ENDPLOT,- 		^\END_PLOT\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ENDSTATE,-  		^\end of statement\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ENDTABLE,-  		^\END_TABLE\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_EQUALS,-  		^\"="\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FIELDLST,-  		^\field name(s)\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FIELDNAM,-  		^\field name\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FIELDOPT,-  		^\field option or period\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FILENAME,-  		^\file name\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FILEOP,-  		^\DEFINE file option\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FORMNAME,-  		^\FMS form name\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FRMREC,-  		^\FORM RECORD name\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FRMRECLS,-  		^\FORM RECORD LIST name\% $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FROM,- 	 		^\FROM\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FRORRCLS,- ) 		^\FORM REC�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           }�N� $      
DTRJA061.C                     �  
  [C]DTRTEXT.MAR;1                                                                                                               Q     $                         � "            ORD name or RECORD LIST name\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_HEADSEG,- 		^\quoted header segment\% $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_HELP,-  		^\help topic\ # $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_IN,-  		^\IN\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_KEYOPT,-  		^\key option\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_KEYWORD,- 		^\KEYWORD\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_LEADTRAL,-  		^\LEADING or TRAILING\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_LINEPAGE,-  		^\LINES_PAGE\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_LPAREN,-  		^\opening parenthesis\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_MAXLINES,-  		^\MAX_LINES\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_NODESPEC,-  		^\Node Specification\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_NOTDVAR,- 		^\variable not declared\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_NUMBER,-  		^\number\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_NXTLISEL,-  		^\next element in list\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OBJNAME,- 		^\object name\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OCCFROM,- 		^\OCCURS or FROM\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OCCURS,-  		^\OCCURS clause\# $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OF,-  		^\OF\ # $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ON,- 	 		^\"ON"\ % $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PAGE,- 	 		^\PAGE\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PERIOD,-  		^\"."\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PICREP,-   		^\picture string repeat count\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PICTURE,- 		^\picture or edit string\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PLOTNAME,-  		^\plot name\& $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PRINT,-
 		^\PRINT\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_PROJLIST,- " 		^\list of values to make unique\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_QUERNAME,-  		^\query name\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_REDEFSUB,-  		^\REDEFINE subcommand\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RELATOP,-' 		^\relational operator (EQ, GT, etc.)\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RELSHIPS,-  		^\RELATIONSHIPS\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_REPNAME,- 		^\REPORT_NAME\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_REPORT,-  		^\REPORT\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_REPSTATE,-  		^\REPORT statement\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RPAREN,-  		^\closing parenthesis\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RPFNAME,-  		^\REPORT, PAGE, or Field Name\$ $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RSE,-( 		^\name of domain, collection, or list\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SCHEMA,-  		^\DBMS schema name\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SETLIST,- 		^\set list\ ( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SETNAME,- 		^\set name\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SETNUM,-  		^\NUMBER\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SETOPT,-  		^\SET option\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SETVALUE,-  		^\a SET value\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SHOWOPT,- 		^\SHOW option\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SNDORRCV,-  		^\SEND or RECEIVE statement\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SORTLIST,-  		^\sort list\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SOURCLAU,-  		^\data source clause\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_STATMENT,-  		^\statement\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_STATOREN,-  		^\statement or "END"\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SUBSCHEM,-  		^\DBMS subschema name\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_SYNONYM,- 		^\SYNONYM\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TERMQUOT,-  		^\closing quote\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TIMEORTO,-  		^\TIMES or TO\# $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TO,-  		^\TO\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TOOITEMS,-  		^\too many control items\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TOPORBOT,-  		^\TOP or BOTTOM\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_TOTCOUN,- 		^\TOTAL or COUNT\ & $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_USAGE,- 		^\USAGE type\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_USEORRET,-  		^\USING or RETRIEVE\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_USERNAME,-  		^\VMS username\ & $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_USING,- 		^\"USING statement" clause\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_VALUEX,-  		^\value expression\ % $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ZERO,- 	 		^\ZERO\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_DOCFMT,-  		^\document format\( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ATTNAME,- 		^\print attribute name\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ATTOPT,-  		^\attribute option\E( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_RENDOPT,- 		^\rendition attribute option\+' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ATTVAL,-E 		^\attribute value\) $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_FIECONST,-9 		^\field name or constant\ % $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_UNIT,-s 		^\"IN" or "MM"\h( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_NUMPRMP,- 		^\number or prompt\d' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_QUOSTR,-  		^\quoted string\' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_ORIENT,-s 		^\"PORTRAIT" or "LANDSCAPE"\( $DTR$TEXT_ENTRY DTR$$K_TXT_REP_AVERAGE,- 		^\AVERAGE\& $DTR$TEXT_ENTRY DTR$$K_TXT_REP_COUNT,-
 		^\COUNT\$ $DTR$TEXT_ENTRY DTR$$K_TXT_REP_MAX,- 		^\MAX\$ $DTR$TEXT_ENTRY DTR$$K_TXT_REP_MIN,- 		^\MIN\% $DTR$TEXT_ENTRY DTR$$K_TXT_REP_PAGE,-s	 		^\Page\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_REP_RUNCOUNT,-E 		^\RUNNING_COUNT\' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_RUNTOT,-l 		^\RUNNING_TOTAL\) $DTR$TEXT_ENTRY DTR$$K_TXT_REP_STANDDEV,-n 		^\STANDARD_DEVIATION\y& $DTR$TEXT_ENTRY DTR$$K_TXT_REP_TOTAL,-
 		^\TOTAL\( $DTR$TEXT_ENTRY DTR$$K_TXT_REP_NEWPAGE,- 		^\<< NEW_PAGE >>\E( $DTR$TEXT_ENTRY DTR$$K_TXT_REP_NEWSECT,- 		^\<< NEW_SECTION >>\' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_DETAIL,-, 		^\Detail Line\& $DTR$TEXT_ENTRY DTR$$K_TXT_REP_TOPOF,- 		^\Top of \& $DTR$TEXT_ENTRY DTR$$K_TXT_REP_BOTOF,- 		^\Bottom of \ * $DTR$TEXT_ENTRY DTR$$K_TXT_REP_BREAKPAGE,-	 		^\Page\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_REPORT,-x 		^\Report\-) $DTR$TEXT_ENTRY DTR$$K_TXT_REP_FUNCTION,-- 		^\Function\T' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_CHOICE,-+ 		^\Choice\+) $DTR$TEXT_ENTRY DTR$$K_TXT_REP_CONSTANT,-  		^\Constant\ % $DTR$TEXT_ENTRY DTR$$K_TXT_REP_EXPR,-a 		^\Expression\s' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_STRING,-  		^\String\e' $DTR$TEXT_ENTRY DTR$$K_TXT_REP_PROMPT,-- 		^\Prompt\-' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_ACCEXT,-R 		^\extend\D' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_ACCMOD,-$ 		^\modify\G( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_ACCREAD,-	 		^\read\-) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_ACCWRITE,-T
 		^\write\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_CONCURR,- 		^\concurrency\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_CONSIST,- 		^\consistency\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DICDOM,-\ 		^\domain\T( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DICPROC,- 		^\procedure\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DICREC,-T 		^\record\$' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DICTAB,-R
 		^\table\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPCOMP,-T 		^\<Computed value>\\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPDATE,-N
 		^\<Date\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPIDX,- 		^\, indexed key>\X) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPIXAE,-Y 		^\, alternate key>\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPIXAG,-T 		^\<Alternate key>\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPIXPE,-T 		^\, primary key>\	) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPIXPG,-T 		^\<Primary key>\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPLIST,-	 		^\<List>\R( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPNBR,- 		^\<Number\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPSEG,- 		^\<Segmented string\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPSTR,- 		^\<Character string\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPUNDE,-N 		^\<Undefined\_) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_DTYPXFLD,-R 		^\<Indexed field>\& $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJDB,- 		^\DATABASE\N) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJDBREC,-T 		^\Record\E) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJDITAB,-$ 		^\Dictionary table\\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJDOM,-$ 		^\Domain\I( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJDTAB,- 		^\Domain table\N' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_OBJREL,-T 		^\Relation\L( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTCMD,-@ 		^\Control       - may issue DEFINEP, DELETEP, SHOWP commands.\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTEXT,-/ 		^\Extend        - may create directory node.\T) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTFORW,-L+ 		^\Forward      - may create subdirectory\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTGDEL,-$> 		^\Global Delete - may delete present node and its children.\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTHEAD,-T 		^\Privileges for\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTHIST,- = 		^\History       - may add entries to node''s history list.\R) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTLDEL,-p, 		^\Local Delete  - may delete present nod.\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTMOD,-0 		^\DTR Modify    - may ready for READ, MODIFY.\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTNACC,-T 		^\No access.\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTPASS,-P( 		^\Pass Thru     - may pass thru node.\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTREAD,-D> 		^\DTR Read      - may ready for READ, use SHOW and EXTRACT.\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTSEE,-2 		^\See           - may see (read) terminal node.\) $DTR�                                                                                                                                                                                                                                                   !                        �~/- $      
DTRJA061.C                     �  
  [C]DTRTEXT.MAR;1                                                                                                               Q     $                         �} 
            $TEXT_ENTRY DTR$$K_TXT_SHO_PROTUPDA,-p- 		^\Update       - may update terminal node.\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTWRIT,-KA 		^\DTR Write     - may ready for READ, WRITE, EXTEND, or WRITE.\K) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_PROTXTND,-tM 		^\DTR Extend/Exe- may ready for EXTEND, execute procedure, access a table.\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_RMSIDX,-D 		^\indexed\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_RMSREL,-  		^\relative\D' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_RMSSEQ,-R 		^\sequential\E( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SEGMENT,- 		^\Segmented string\_( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETAUTO,- 		^\automatic\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETFIX,-D
 		^\fixed\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETMAN,-  		^\manual\N( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETMAND,- 		^\mandatory\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETMEM,-  		^\Member:\' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETNAM,- 	 		^\Set:\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETOPT,-  		^\optional\e' $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SETOWN,-D 		^\Owner:\i( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SHARDEF,- 		^\\P( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SHAREXC,- 		^\exclusive\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SHARPROT,-T 		^\protected\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SHARSHAR,-  		^\shared\N) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_SHARSNAP,-m 		^\snapshot\X) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPEDBMS,-R	 		^\DBMS\ ) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPEPORT,- 	 		^\PORT\R( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPERDB,- 		^\Rdb\( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPEREM,- 		^\REMOTE\L( $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPERMS,- 		^\RMS\) $DTR$TEXT_ENTRY DTR$$K_TXT_SHO_TYPEVIEW,-\	 		^\VIEW\e$ $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_ADT,- 		^\ADT\# $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_AT,-  		^\@\& $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_CLOSE,-
 		^\CLOSE\% $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_EDIT,-X	 		^\EDIT\$) $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_EDITBACK,-X 		^\EDIT_BACKUP\) $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_EDITSHOW,- 	 		^\EDIT\N% $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_EXIT,-E	 		^\EXIT\D& $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_GUIDE,-
 		^\GUIDE\( $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HELINES,- 		^\HELP_LINES\	% $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HELP,-T	 		^\HELP\E& $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HELPQ,- 		^\?\) $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HELPSHOW,-d	 		^\HELP\D) $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HEPROMPT,-\ 		^\HELP_PROMPT\) $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_HEWINDOW,-t 		^\HELP_WINDOW\% $DTR$TEXT_ENTRY DTR$$K_TXT_UDK_OPEN,-E	 		^\OPEN\e .END��( $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OCCFROM,- 		^\OCCURS or FROM\ ' $DTR$TEXT_ENTRY DTR$$K_TXT_PRM_OCCURS,-  		^\OC�               * [C]DTRTXTLIB.MLB;1 +  , �   . ]    / s P   4     ]   \                     - 
    0  � 1    2   3      K � P   W   O \    5   6 `�qn�*�  7 �z7*�*�  8          9          G s P  H �� J �                              �%�   VAX-11 Librarian V04-00        `<�k�*� ��pl�*�                 Z   �\         
                          [     [   N                                                                                                                                                                                                                                                                                                                                                                                   D           0   � $DTR$TEXT_ENTRY    $DTR$TEXT_INIT0   �$DTR$TXT_CONSTANTS                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     �     ��Bl�*�  	.MACRO $DTR$TEXT_INIT   $DTR$TXT_CONSTANTS  ) DTR$$TEXT_TABLE :: .BLKW DTR$$K_TXT_TOTAL                     TAIL = .   & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ABORT,- 				^\ABORT\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ADVANCED,-  				^\ADVANCED\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_AFTER,- 				^\AFTER\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ALL,-
 				^\ALL\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ALLOCAT,- 				^\ALLOCATION\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ALMAJMIN,-  				^\ALIGNED_MAJOR_MINOR\$ 	$DTR$TE�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  "                        ,�� $      
DTRJA061.C                     �  
  [C]DTRTXTLIB.MLB;1                                                                                                                  ]                         �� "                XT_ENTRY DTR$$K_TXT_KW_AND,-
 				^\AND\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ANY,-
 				^\ANY\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ARGS,-  				^\ARGUMENTS\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_AS,- 	 				^\AS\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ASC,-
 				^\ASC\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ASCEND,-  				^\ASCENDING\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_AT,- 	 				^\AT\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ATT,-
 				^\ATT\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_AVERAGE,- 				^\AVERAGE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BANG,-      				^\!\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BANISH,-  				^\BANISH\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BATCH,- 				^\BATCH\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BEFOR,- 				^\BEFORE\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BEGIN,- 				^\BEGIN\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BETWEEN,- 				^\BETWEEN\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BLANK,- 				^\BLANK\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BOLD,-  				^\BOLD\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BOOLEAN,- 				^\BOOLEAN\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BOTTOM,-  				^\    BOTTOM\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BOX,-
 				^\BOX\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BTWEEN,- 	 				^\BT\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BUT,-
 				^\BUT\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BY,- 	 				^\BY\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_BYTE,-  				^\BYTE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CASE,-  				^\CHOICE\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CDO,-
 				^\CDO\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CHANGE,-  				^\CHANGE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CHAR,-  				^\CHARACTER\& 	$DTR$TEXT_ENTRY     DTR$$K_TXT_KW_CHARS,- 				^\CHARACTERS\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COL,- 				^\COLUMN\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COLHEAD,- 				^\COLUMN_HEADER\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COLLECT,- 				^\COLLECTIONS\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COLON,- 				^\:\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COLSPAGE,-  				^\COLUMNS_PAGE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COLUMN,- 
 				^\COL\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMMA,- 				^\,\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMMIT,-  				^\COMMIT\ % 	$DT    R$TEXT_ENTRY DTR$$K_TXT_KW_COMP,-  				^\COMP\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMP1,- 				^\COMP_1\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMP2,- 				^\COMP_2\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMP3,- 				^\COMP_3\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMP5,- 				^\COMP_5\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMP6,- 				^\COMP_6\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COMPUTED,-  				^\COMPUTED\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CONNECT,- 				^\CONNECT\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CONT,-  				^\CONT\ ( 	$    DTR$TEXT_ENTRY DTR$$K_TXT_KW_CONTAIN,- 				^\CONTAINING\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_COUNT,- 				^\COUNT\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CROSS,- 				^\CROSS\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CURRENCY,-  				^\CURRENCY\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_CURRENT,- 				^\CURRENT\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DASH,-  				^\-\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DATABASE,-  				^\DATABASE\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DATABASS,-  				^\DATABASES\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DATATYPE,-      				^\DATATYPE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DATE,-  				^\DATE\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DEBUG,- 				^\DEBUG\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DECIMAL,- 				^\DECIMAL\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DECLARE,- 				^\DECLARE\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DECLATT,- 				^\DECLARE_ATT\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DECREASE,-  				^\DECREASING\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DEFAULT,- 				^\DEFAULT\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DEFINE,-  				^\DEFINE\ ( 	$DTR$TEXT_ENTRY DTR$$    K_TXT_KW_DEFINEP,- 				^\DEFINEP\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DELETE,-  				^\DELETE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DELETEP,- 				^\DELETEP\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DENY,-  				^\DENY\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DEPEND,-  				^\DEPENDING\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DESC,-  				^\DESC\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DESCEND,- 				^\DESCENDING\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DFLOAT,-  				^\D_FLOATING\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DICTION,- 				^\DICTIONARY\ )     	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DICTIONS,-  				^\DICTIONARIES\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DIGIT,- 				^\DIGIT\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DIGITS,-  				^\DIGITS\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DISCON,-  				^\DISCONNECT\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DISPLAY,- 				^\DISPLAY\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DISPLAYF,-  				^\DISPLAY_FORM\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DO,- 	 				^\DO\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DOMAIN,-  				^\DOMAIN\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DOM    AINS,- 				^\DOMAINS\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DOUBLE,-  				^\DOUBLE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DQUOTE,-  				^\"\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DROP,-  				^\DROP\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_DUP,-
 				^\DUP\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ELSE,-  				^\ELSE\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_END,-
 				^\END\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDCASE,- 				^\END_CHOICE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDING,-  				^\ENDING\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDPLOT,- 		    		^\END_PLOT\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDPROC,- 				^\END_PROCEDURE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDREP,-  				^\END_REPORT\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENDTAB,-  				^\END_TABLE\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ENTRY,- 				^\ENTRY\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EQ,- 	 				^\EQ\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EQUAL,- 				^\EQUAL\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EQUALS,-  				^\=\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ERASE,- 				^\ERASE\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ESTRING,-     				^\EDIT_STRING\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EXCLUS,-  				^\EXCLUSIVE\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EXECUTE,- 				^\EXECUTE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EXTEND,-  				^\EXTEND\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_EXTRACT,- 				^\EXTRACT\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FAMILY,-  				^\FAMILY\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FFLOAT,-  				^\F_FLOATING\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FIELD,- 				^\FIELD\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FIELDS,-  				^\FIELDS\ % 	$DTR$TEXT_ENTRY D    TR$$K_TXT_KW_FILE,-  				^\FILE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FILL,-  				^\FILL\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FILLER,-  				^\FILLER\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FIND,-  				^\FIND\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FINISH,-  				^\FINISH\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FIRST,- 				^\FIRST\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FOR,-
 				^\FOR\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FORM,-  				^\FORM\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FORMAT,-  				^\FORMAT\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW    _FORMS,- 				^\FORMS\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_FROM,-  				^\FROM\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GE,- 	 				^\GE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GETFORM,- 				^\GET_FORM\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GFLOAT,-  				^\G_FLOATING\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GRANT,- 				^\GRANT\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GREATEQ,- 				^\GREATER_EQUAL\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GREATHAN,-  				^\GREATER_THAN\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GROUP,- 				^\GROUP\' 	$DTR$TEXT_ENTRY D    TR$$K_TXT_KW_GRTHAN,-  				^\>\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_GT,- 	 				^\GT\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_HFLOAT,-  				^\H_FLOATING\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_IF,- 	 				^\IF\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_IN,- 	 				^\IN\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_INCR,-  				^\INCR\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_INCREASE,-  				^\INCREASING\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_INSERT,-  				^\INSERT\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_INTEGER,- 				^\INTEGER\# 	$DTR$TEXT_ENTRY DTR$$K_TX    T_KW_IS,- 	 				^\IS\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ITALIC,-  				^\ITALIC\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_JUST,-  				^\JUST\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_JUSTIFY,- 				^\JUSTIFY\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_JUSTIFYD,-  				^\JUSTIFIED\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_KEEP,-  				^\KEEP\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_KEY,-
 				^\KEY\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_KEYWORD,- 				^\KEYWORD\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LANDSC,-  				^\LANDSCAPE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_    KW_LAST,-  				^\LAST\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LCK_WAIT,-  				^\LOCK_WAIT\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LE,- 	 				^\LE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LEADING,- 				^\LEADING\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LEAVE,- 				^\LEAVE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LEFT,-  				^\LEFT\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LEFTRIGH,-  				^\LEFT_RIGHT\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LESSEQ,-  				^\LESS_EQUAL\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LESSTHAN,-  				^\LESS_THAN\) 	$DTR$TEXT_ENT�                                                                                                                                                                                                                                                   #                        h~�! $      
DTRJA061.C                     �  
  [C]DTRTXTLIB.MLB;1                                                                                                                  ]                         69 "                RY DTR$$K_TXT_KW_LINEPAGE,-  				^\LINES_PAGE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LIST,-  				^\LIST\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LOCAL,- 				^\LOCAL\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LONG,-  				^\LONG\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LONGWORD,-  				^\LONGWORD\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LPAREN,-  				^\(\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LT,- 	 				^\LT\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_LTHAN,- 				^\<\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MAJMIN,-  				^\MAJOR_MINOR\& 	$DTR$TEXT_ENTRY D    TR$$K_TXT_KW_MATCH,- 				^\MATCH\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MAX,-
 				^\MAX\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MAXLINES,-  				^\MAX_LINES\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MAXPAGES,-  				^\MAX_PAGES\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MEMBER,-  				^\MEMBER\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MIN,-
 				^\MIN\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MISSING,- 				^\MISSING\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MM,- 	 				^\MM\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_MODIFY,-  				^\MODIFY\ # 	$DTR$TEXT_ENTRY DTR$$     K_TXT_KW_NE,- 	 				^\NE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NETWORK,- 				^\NETWORK\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NEWPAGE,- 				^\NEW_PAGE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NEWSECT,- 				^\NEW_SECTION\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NEXT,-  				^\NEXT\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NO,- 	 				^\NO\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NONE,-  				^\NONE\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NONLOCAL,-  				^\NONLOCAL\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NOT,-
 				^\NOT\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_ !   KW_NOTEQ,- 				^\NOT_EQUAL\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NOVER,- 				^\NOVERIFY\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NUMBER,-  				^\NUMBER\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_NUMERIC,- 				^\NUMERIC\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OCCURS,-  				^\OCCURS\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OCTA,-  				^\OCTA\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OCTAWORD,-  				^\OCTAWORD\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OF,- 	 				^\OF\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ON,- 	 				^\ON\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_ "   KW_OR,- 	 				^\OR\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OTHERW,-  				^\OTHERWISE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OVER,-  				^\OVER\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OVERPUN,- 				^\OVERPUNCHED\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_OWNER,- 				^\OWNER\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PACKED,-  				^\PACKED\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PAGE,-  				^\PAGE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PAPERH,-  				^\PAPER_HEIGHT\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PAPERO,-  				^\PAPER_ORIENTATION\' 	$DTR$TEXT #   _ENTRY DTR$$K_TXT_KW_PAPERS,-  				^\PAPER_SIZE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PAPERW,-  				^\PAPER_WIDTH\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PATH,-  				^\PATH\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PERIOD,-  				^\.\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PIC,-
 				^\PIC\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PICTURE,- 				^\PICTURE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PLOT,-  				^\PLOT\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PLOTS,- 				^\PLOTS\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PLUS,-  				^\+\% 	$DTR$TEXT_ENTRY DTR $   $$K_TXT_KW_PORT,-  				^\PORT\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PORTRAIT,-  				^\PORTRAIT\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PRINT,- 				^\PRINT\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PRIOR,- 				^\PRIOR\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PRIVS,- 				^\PRIVILEGES\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PROC,-  				^\PROCEDURE\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PROCS,- 				^\PROCEDURES\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PROMPT,-  				^\PROMPT\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PROTECTD,-  				^\PROTECTED\& 	$DT %   R$TEXT_ENTRY DTR$$K_TXT_KW_PURGE,- 				^\PURGE\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PUTFORM,- 				^\PUT_FORM\ # 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_PW,- 	 				^\PW\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_QUAD,-  				^\QUAD\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_QUADWORD,-  				^\QUADWORD\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_QUERHEAD,-  				^\QUERY_HEADER\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_QUERNAME,-  				^\QUERY_NAME\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_READ,-  				^\READ\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_READY,- 				^\ &   READY\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REAL,-  				^\REAL\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RECEIVE,- 				^\RECEIVE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RECONN,-  				^\RECONNECT\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RECORD,-  				^\RECORD\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RECORDS,- 				^\RECORDS\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RECTRLT,- 				^\RECEIVE_CONTROL_TEXT\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REDEFINE,-  				^\REDEFINE\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REDEFS,-  				^\REDEFINES\' 	$DTR$TEXT_ENTRY '    DTR$$K_TXT_KW_REDUCE,-  				^\REDUCE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REDUCED,- 				^\REDUCED\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RELEASE,- 				^\RELEASE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REPEAT,-  				^\REPEAT\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REPHEAD,- 				^\REPORT_HEADER\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REPNAME,- 				^\REPORT_NAME\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REPORT,-  				^\REPORT\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RESET,- 				^\RESET\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RETRIEVE,-  				^\RE (   TRIEVE\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_REVERSE,- 				^\REVERSE\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RIGHT,- 				^\RIGHT\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ROLLBACK,-  				^\ROLLBACK\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RPAREN,-  				^\)\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RSE,-
 				^\RSE\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_RUNNING,- 				^\RUNNING\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SCALE,- 				^\SCALE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SCHEMA,-  				^\SCHEMA\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SCHEMAS,- 				^\SC )   HEMAS\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SEARCH,-  				^\SEARCH\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SECTRLT,- 				^\SEND_CONTROL_TEXT\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SELECT,-  				^\SELECT\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SEMICOL,- 				^\SEMICOLON\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SEND,-  				^\SEND\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SEPARATE,-  				^\SEPARATE\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SET,-
 				^\SET\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SETS,-  				^\SETS\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SET *   UP,- 				^\SET_UP\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SHARED,-  				^\SHARED\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SHOW,-  				^\SHOW\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SHOWP,- 				^\SHOWP\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SIGN,-  				^\SIGN\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SIGNED,-  				^\SIGNED\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SIZE,-  				^\SIZE\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SKIP,-  				^\SKIP\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SLASH,- 				^\/\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SNAPSHOT,-  				 +   ^\SNAPSHOT\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SORT,-  				^\SORT\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SORTED,-  				^\SORTED\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SOURCE,-  				^\SOURCE\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SPACE,- 				^\SPACE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STAR,-  				^\*\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STARTING,-  				^\STARTING\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STDDEV,-  				^\STD_DEV\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STORE,- 				^\STORE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STRING,-  		 ,   		^\STRING\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_STRUCT,-  				^\STRUCTURE\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SUBSCHEM,-  				^\SUBSCHEMA\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SUM,-
 				^\SUM\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SUPERCED,-  				^\SUPERCEDE\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SUPERSED,-  				^\SUPERSEDE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SYNC,-  				^\SYNC\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SYNCHRO,- 				^\SYNCHRONIZED\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SYNONYM,- 				^\SYNONYM\) 	$DTR$TEXT_ENTRY DTR -   $$K_TXT_KW_SYNONYMS,-  				^\SYNONYMS\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TAB,-
 				^\TAB\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TABLE,- 				^\TABLE\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TABLES,-  				^\TABLES\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TERMINAL,-  				^\TERMINAL\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TEXT,-  				^\TEXT\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_THE,-
 				^\THE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_THEN,-  				^\THEN\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TIMES,- 				^\TIMES\# 	$DTR$TEXT_ENTRY DTR$$K_TXT_ .   KW_TO,- 	 				^\TO\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TOP,-
 				^\TOP\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TOTAL,- 				^\TOTAL\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TRAILING,-  				^\TRAILING\ $ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_UIC,-
 				^\UIC\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_UNDERLN,- 				^\UNDERLINE\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_UNSIGNED,-  				^\UNSIGNED\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_USAGE,- 				^\USAGE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_USER,-  				^\USER\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_USING,-�                                                                                                                                                                                                                                                   $                        �2a� $      
DTRJA061.C                     �  
  [C]DTRTXTLIB.MLB;1                                                                                                                  ]                         �5 "     .        /    				^\USING\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VALID,- 				^\VALID\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VALUE,- 				^\VALUE\) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VARIABLS,-  				^\VARIABLES\( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VARYING,- 				^\VARYING\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VECTOR,-  				^\VECTOR\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VERIFY,-  				^\VERIFY\ ( 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VERTBAR,- 				^\|\$ 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_VIA,-
 				^\VIA\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WHEN,-  				 0   ^\WHEN\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WHILE,- 				^\WHILE\& 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WIDTH,- 				^\WIDTH\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WITH,-  				^\WITH\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WITHFORM,-  				^\WITH_FORM\' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WITHIN,-  				^\WITHIN\ % 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WORD,-  				^\WORD\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_WRITE,- 				^\WRITE\% 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ZERO,-  				^\ZERO\ & 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_ZONED,- 				^\ZONED\ 1   ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_SFLOAT,-  				^\S_FLOATING\ ' 	$DTR$TEXT_ENTRY DTR$$K_TXT_KW_TFLOAT,-  				^\T_FLOATING\ ) 	$DTR$TEXT_ENTRY DTR$$K_TXT_MSC_CURRENT,-  				^\CURRENT\   	.ENDM $DTR$TEXT_INIT  w w   �     @fl�*� " 	.MACRO $DTR$TEXT_ENTRY NAME, TEXT 	     ADDR = TAIL  	     . = TAIL 	     .ASCIC \TEXT\  	     TAIL = .& 	     . = DTR$$TEXT_TABLE + <2 * NAME>) 	     .SIGNED_WORD ADDR - DTR$$TEXT_TABLE  	.ENDM $DTR$TEXT_ENTRY w w   �     ��pl�*� J 	.MACRO	$DTR$TXT_CONSTANTS,. 2   .EQU=<=>,..COL=<:> ; IDENT VAX DATATRIEVE V6.1 DTR$$K_TXT_DAT_APR'..equ'0 DTR$$K_TXT_DAT_AUG'..equ'1 DTR$$K_TXT_DAT_DEC'..equ'2 DTR$$K_TXT_DAT_FEB'..equ'3 DTR$$K_TXT_DAT_FRI'..equ'4 DTR$$K_TXT_DAT_JAN'..equ'5 DTR$$K_TXT_DAT_JUL'..equ'6 DTR$$K_TXT_DAT_JUN'..equ'7 DTR$$K_TXT_DAT_MAR'..equ'8 DTR$$K_TXT_DAT_MAY'..equ'9 DTR$$K_TXT_DAT_MON'..equ'10  DTR$$K_TXT_DAT_NOV'..equ'11  DTR$$K_TXT_DAT_NOW'..equ'12  DTR$$K_TXT_DAT_OCT'..equ'13  DTR$$K_TXT_DAT_PIC'..equ'14  DTR$$K_TXT_DAT_SAT'..equ'15  3    DTR$$K_TXT_DAT_SEP'..equ'16  DTR$$K_TXT_DAT_SUN'..equ'17  DTR$$K_TXT_DAT_THU'..equ'18  DTR$$K_TXT_DAT_TODAY'..equ'19   DTR$$K_TXT_DAT_TOMORROW'..equ'20 DTR$$K_TXT_DAT_TUE'..equ'21  DTR$$K_TXT_DAT_WED'..equ'22  DTR$$K_TXT_DAT_YESTER'..equ'23 DTR$$K_TXT_DAT_SPARE0'..equ'24 DTR$$K_TXT_DAT_SPARE1'..equ'25 DTR$$K_TXT_DAT_SPARE2'..equ'26 DTR$$K_TXT_DAT_SPARE3'..equ'27 DTR$$K_TXT_DAT_SPARE4'..equ'28 DTR$$K_TXT_DAT_SPARE5'..equ'29 DTR$$K_TXT_DAT_SPARE6'..equ'30 DTR$$K_TXT_DAT_SPARE7'..equ'31  4   DTR$$K_TXT_DAT_SPARE8'..equ'32 DTR$$K_TXT_DAT_SPARE9'..equ'33 DTR$$K_TXT_KW_ABORT'..equ'34 DTR$$K_TXT_KW_ADVANCED'..equ'35  DTR$$K_TXT_KW_AFTER'..equ'36 DTR$$K_TXT_KW_ALL'..equ'37 DTR$$K_TXT_KW_ALLOCAT'..equ'38 DTR$$K_TXT_KW_ALMAJMIN'..equ'39  DTR$$K_TXT_KW_AND'..equ'40 DTR$$K_TXT_KW_ANY'..equ'41 DTR$$K_TXT_KW_ARGS'..equ'42  DTR$$K_TXT_KW_AS'..equ'43  DTR$$K_TXT_KW_ASC'..equ'44 DTR$$K_TXT_KW_ASCEND'..equ'45  DTR$$K_TXT_KW_AT'..equ'46  DTR$$K_TXT_KW_ATT'..equ'47 DTR$$K_TXT_KW_AVERAGE'.. 5   equ'48 DTR$$K_TXT_KW_BANG'..equ'49  DTR$$K_TXT_KW_BANISH'..equ'50  DTR$$K_TXT_KW_BATCH'..equ'51 DTR$$K_TXT_KW_BEFOR'..equ'52 DTR$$K_TXT_KW_BEGIN'..equ'53 DTR$$K_TXT_KW_BETWEEN'..equ'54 DTR$$K_TXT_KW_BLANK'..equ'55 DTR$$K_TXT_KW_BOLD'..equ'56  DTR$$K_TXT_KW_BOOLEAN'..equ'57 DTR$$K_TXT_KW_BOTTOM'..equ'58  DTR$$K_TXT_KW_BOX'..equ'59 DTR$$K_TXT_KW_BTWEEN'..equ'60  DTR$$K_TXT_KW_BUT'..equ'61 DTR$$K_TXT_KW_BY'..equ'62  DTR$$K_TXT_KW_BYTE'..equ'63  DTR$$K_TXT_KW_CASE'..equ'64  DTR$$K_TXT_KW_ 6   CDO'..equ'65 DTR$$K_TXT_KW_CHANGE'..equ'66  DTR$$K_TXT_KW_CHAR'..equ'67  DTR$$K_TXT_KW_CHARS'..equ'68 DTR$$K_TXT_KW_COL'..equ'69 DTR$$K_TXT_KW_COLHEAD'..equ'70 DTR$$K_TXT_KW_COLLECT'..equ'71 DTR$$K_TXT_KW_COLON'..equ'72 DTR$$K_TXT_KW_COLSPAGE'..equ'73  DTR$$K_TXT_KW_COLUMN'..equ'74  DTR$$K_TXT_KW_COMMA'..equ'75 DTR$$K_TXT_KW_COMMIT'..equ'76  DTR$$K_TXT_KW_COMP'..equ'77  DTR$$K_TXT_KW_COMP1'..equ'78 DTR$$K_TXT_KW_COMP2'..equ'79 DTR$$K_TXT_KW_COMP3'..equ'80 DTR$$K_TXT_KW_COMP5'..equ'81  7   DTR$$K_TXT_KW_COMP6'..equ'82 DTR$$K_TXT_KW_COMPUTED'..equ'83  DTR$$K_TXT_KW_CONCURR'..equ'84 DTR$$K_TXT_KW_CONNECT'..equ'85 DTR$$K_TXT_KW_CONSIST'..equ'86 DTR$$K_TXT_KW_CONT'..equ'87  DTR$$K_TXT_KW_CONTAIN'..equ'88 DTR$$K_TXT_KW_COUNT'..equ'89 DTR$$K_TXT_KW_CROSS'..equ'90 DTR$$K_TXT_KW_CURRENCY'..equ'91  DTR$$K_TXT_KW_CURRENT'..equ'92 DTR$$K_TXT_KW_DASH'..equ'93  DTR$$K_TXT_KW_DATABASE'..equ'94  DTR$$K_TXT_KW_DATABASS'..equ'95  DTR$$K_TXT_KW_DATATYPE'..equ'96  DTR$$K_TXT_KW_DATE'..equ'9 8   7  DTR$$K_TXT_KW_DEBUG'..equ'98 DTR$$K_TXT_KW_DECIMAL'..equ'99 DTR$$K_TXT_KW_DECLARE'..equ'100  DTR$$K_TXT_KW_DECLATT'..equ'101   DTR$$K_TXT_KW_DECREASE'..equ'102 DTR$$K_TXT_KW_DEFAULT'..equ'103  DTR$$K_TXT_KW_DEFINE'..equ'104 DTR$$K_TXT_KW_DEFINEP'..equ'105  DTR$$K_TXT_KW_DELETE'..equ'106 DTR$$K_TXT_KW_DELETEP'..equ'107  DTR$$K_TXT_KW_DENY'..equ'108 DTR$$K_TXT_KW_DEPEND'..equ'109 DTR$$K_TXT_KW_DESC'..equ'110 DTR$$K_TXT_KW_DESCEND'..equ'111  DTR$$K_TXT_KW_DFLOAT'..equ'112 DTR$$K_TXT_KW_ 9   DICTION'..equ'113   DTR$$K_TXT_KW_DICTIONS'..equ'114 DTR$$K_TXT_KW_DIGIT'..equ'115  DTR$$K_TXT_KW_DIGITS'..equ'116 DTR$$K_TXT_KW_DISCON'..equ'117 DTR$$K_TXT_KW_DISPLAY'..equ'118   DTR$$K_TXT_KW_DISPLAYF'..equ'119 DTR$$K_TXT_KW_DO'..equ'120 DTR$$K_TXT_KW_DOMAIN'..equ'121 DTR$$K_TXT_KW_DOMAINS'..equ'122  DTR$$K_TXT_KW_DOUBLE'..equ'123 DTR$$K_TXT_KW_DQUOTE'..equ'124 DTR$$K_TXT_KW_DROP'..equ'125 DTR$$K_TXT_KW_DUP'..equ'126  DTR$$K_TXT_KW_ELSE'..equ'127 DTR$$K_TXT_KW_END'..equ'128  DTR$$K_TXT :   _KW_ENDCASE'..equ'129  DTR$$K_TXT_KW_ENDING'..equ'130 DTR$$K_TXT_KW_ENDPLOT'..equ'131  DTR$$K_TXT_KW_ENDPROC'..equ'132  DTR$$K_TXT_KW_ENDREP'..equ'133 DTR$$K_TXT_KW_ENDTAB'..equ'134 DTR$$K_TXT_KW_ENTRY'..equ'135  DTR$$K_TXT_KW_EQ'..equ'136 DTR$$K_TXT_KW_EQUAL'..equ'137  DTR$$K_TXT_KW_EQUALS'..equ'138 DTR$$K_TXT_KW_ERASE'..equ'139  DTR$$K_TXT_KW_ESTRING'..equ'140  DTR$$K_TXT_KW_EXCLUS'..equ'141 DTR$$K_TXT_KW_EXECUTE'..equ'142  DTR$$K_TXT_KW_EXTEND'..equ'143 DTR$$K_TXT_KW_EXTRACT'..equ'14 ;   4  DTR$$K_TXT_KW_FAMILY'..equ'145 DTR$$K_TXT_KW_FFLOAT'..equ'146 DTR$$K_TXT_KW_FIELD'..equ'147  DTR$$K_TXT_KW_FIELDS'..equ'148 DTR$$K_TXT_KW_FILE'..equ'149 DTR$$K_TXT_KW_FILL'..equ'150 DTR$$K_TXT_KW_FILLER'..equ'151 DTR$$K_TXT_KW_FIND'..equ'152 DTR$$K_TXT_KW_FINISH'..equ'153 DTR$$K_TXT_KW_FIRST'..equ'154  DTR$$K_TXT_KW_FOR'..equ'155  DTR$$K_TXT_KW_FORM'..equ'156 DTR$$K_TXT_KW_FORMAT'..equ'157 DTR$$K_TXT_KW_FORMS'..equ'158  DTR$$K_TXT_KW_FROM'..equ'159 DTR$$K_TXT_KW_GE'..equ'160 DTR$$K <   _TXT_KW_GETFORM'..equ'161  DTR$$K_TXT_KW_GFLOAT'..equ'162 DTR$$K_TXT_KW_GRANT'..equ'163  DTR$$K_TXT_KW_GREATEQ'..equ'164   DTR$$K_TXT_KW_GREATHAN'..equ'165 DTR$$K_TXT_KW_GROUP'..equ'166  DTR$$K_TXT_KW_GRTHAN'..equ'167 DTR$$K_TXT_KW_GT'..equ'168 DTR$$K_TXT_KW_HFLOAT'..equ'169 DTR$$K_TXT_KW_IF'..equ'170 DTR$$K_TXT_KW_IN'..equ'171 DTR$$K_TXT_KW_INCR'..equ'172  DTR$$K_TXT_KW_INCREASE'..equ'173 DTR$$K_TXT_KW_INIVEC'..equ'174 DTR$$K_TXT_KW_INSERT'..equ'175 DTR$$K_TXT_KW_INTEGER'..equ'176  DTR$ =   $K_TXT_KW_IS'..equ'177 DTR$$K_TXT_KW_ITALIC'..equ'178 DTR$$K_TXT_KW_JUST'..equ'179 DTR$$K_TXT_KW_JUSTIFY'..equ'180   DTR$$K_TXT_KW_JUSTIFYD'..equ'181 DTR$$K_TXT_KW_KEEP'..equ'182 DTR$$K_TXT_KW_KEY'..equ'183  DTR$$K_TXT_KW_KEYDEFS'..equ'184  DTR$$K_TXT_KW_KEYPAD'..equ'185 DTR$$K_TXT_KW_KEYWORD'..equ'186  DTR$$K_TXT_KW_LANDSC'..equ'187 DTR$$K_TXT_KW_LAST'..equ'188  DTR$$K_TXT_KW_LCK_WAIT'..equ'189 DTR$$K_TXT_KW_LE'..equ'190 DTR$$K_TXT_KW_LEADING'..equ'191  DTR$$K_TXT_KW_LEAVE'..equ'192  DT >   R$$K_TXT_KW_LEFT'..equ'193  DTR$$K_TXT_KW_LEFTRIGH'..equ'194 DTR$$K_TXT_KW_LESSEQ'..equ'195  DTR$$K_TXT_KW_LESSTHAN'..equ'196  DTR$$K_TXT_KW_LINEPAGE'..equ'197 DTR$$K_TXT_KW_LIST'..equ'198 DTR$$K_TXT_KW_LOCAL'..equ'199  DTR$$K_TXT_KW_LONG'..equ'200  DTR$$K_TXT_KW_LONGWORD'..equ'201 DTR$$K_TXT_KW_LPAREN'..equ'202 DTR$$K_TXT_KW_LT'..equ'203 DTR$$K_TXT_KW_LTHAN'..equ'204  DTR$$K_TXT_KW_MAJMIN'..equ'205 DTR$$K_TXT_KW_MATCH'..equ'206  DTR$$K_TXT_KW_MAX'..equ'207   DTR$$K_TXT_KW_MAXLINES'..equ'208 ?     DTR$$K_TXT_KW_MAXPAGES'..equ'209 DTR$$K_TXT_KW_MEMBER'..equ'210 DTR$$K_TXT_KW_MIN'..equ'211  DTR$$K_TXT_KW_MISSING'..equ'212  DTR$$K_TXT_KW_MM'..equ'213 DTR$$K_TXT_KW_MODIFY'..equ'214 DTR$$K_TXT_KW_NE'..equ'215 DTR$$K_TXT_KW_NETWORK'..equ'216  DTR$$K_TXT_KW_NEWPAGE'..equ'217  DTR$$K_TXT_KW_NEWSECT'..equ'218  DTR$$K_TXT_KW_NEXT'..equ'219 DTR$$K_TXT_KW_NO'..equ'220 DTR$$K_TXT_KW_NONE'..equ'221  DTR$$K_TXT_KW_NONLOCAL'..equ'222 DTR$$K_TXT_KW_NOT'..equ'223  DTR$$K_TXT_KW_NOTEQ'..equ'224  �                                                                                                                                                                                                                                                   %                        �D_� $      
DTRJA061.C                     �  
  [C]DTRTXTLIB.MLB;1                                                                                                                  ]                         }� "     ?        @   DTR$$K_TXT_KW_NOVER'..equ'225  DTR$$K_TXT_KW_NUMBER'..equ'226 DTR$$K_TXT_KW_NUMERIC'..equ'227  DTR$$K_TXT_KW_OCCURS'..equ'228 DTR$$K_TXT_KW_OCTA'..equ'229  DTR$$K_TXT_KW_OCTAWORD'..equ'230 DTR$$K_TXT_KW_OF'..equ'231 DTR$$K_TXT_KW_ON'..equ'232  DTR$$K_TXT_KW_OPTIMIZE'..equ'233 DTR$$K_TXT_KW_OR'..equ'234 DTR$$K_TXT_KW_OTHERW'..equ'235 DTR$$K_TXT_KW_OVER'..equ'236 DTR$$K_TXT_KW_OVERPUN'..equ'237  DTR$$K_TXT_KW_OWNER'..equ'238  DTR$$K_TXT_KW_PACKED'..equ'239 DTR$$K_TXT_KW_PAGE'..equ'240 DTR$ A   $K_TXT_KW_PAPERH'..equ'241 DTR$$K_TXT_KW_PAPERO'..equ'242 DTR$$K_TXT_KW_PAPERS'..equ'243 DTR$$K_TXT_KW_PAPERW'..equ'244 DTR$$K_TXT_KW_PATH'..equ'245 DTR$$K_TXT_KW_PERIOD'..equ'246 DTR$$K_TXT_KW_PIC'..equ'247  DTR$$K_TXT_KW_PICTURE'..equ'248  DTR$$K_TXT_KW_PLOT'..equ'249 DTR$$K_TXT_KW_PLOTS'..equ'250  DTR$$K_TXT_KW_PLUS'..equ'251 DTR$$K_TXT_KW_PORT'..equ'252  DTR$$K_TXT_KW_PORTRAIT'..equ'253 DTR$$K_TXT_KW_PRINT'..equ'254  DTR$$K_TXT_KW_PRIOR'..equ'255  DTR$$K_TXT_KW_PRIVS'..equ'256  DTR$ B   $K_TXT_KW_PROC'..equ'257 DTR$$K_TXT_KW_PROCS'..equ'258  DTR$$K_TXT_KW_PROMPT'..equ'259  DTR$$K_TXT_KW_PROTECTD'..equ'260 DTR$$K_TXT_KW_PURGE'..equ'261  DTR$$K_TXT_KW_PUTFORM'..equ'262  DTR$$K_TXT_KW_PW'..equ'263 DTR$$K_TXT_KW_QUAD'..equ'264  DTR$$K_TXT_KW_QUADWORD'..equ'265  DTR$$K_TXT_KW_QUERHEAD'..equ'266  DTR$$K_TXT_KW_QUERNAME'..equ'267 DTR$$K_TXT_KW_READ'..equ'268 DTR$$K_TXT_KW_READY'..equ'269  DTR$$K_TXT_KW_REAL'..equ'270 DTR$$K_TXT_KW_RECEIVE'..equ'271  DTR$$K_TXT_KW_RECONN'..equ'272 C    DTR$$K_TXT_KW_RECORD'..equ'273 DTR$$K_TXT_KW_RECORDS'..equ'274  DTR$$K_TXT_KW_RECTRLT'..equ'275   DTR$$K_TXT_KW_REDEFINE'..equ'276 DTR$$K_TXT_KW_REDEFS'..equ'277 DTR$$K_TXT_KW_REDUCE'..equ'278 DTR$$K_TXT_KW_REDUCED'..equ'279   DTR$$K_TXT_KW_RELATION'..equ'280 DTR$$K_TXT_KW_RELEASE'..equ'281  DTR$$K_TXT_KW_REPEAT'..equ'282 DTR$$K_TXT_KW_REPHEAD'..equ'283  DTR$$K_TXT_KW_REPLOT'..equ'284 DTR$$K_TXT_KW_REPNAME'..equ'285  DTR$$K_TXT_KW_REPORT'..equ'286 DTR$$K_TXT_KW_RESET'..equ'287   DTR$$K_T D   XT_KW_RETRIEVE'..equ'288 DTR$$K_TXT_KW_REVERSE'..equ'289  DTR$$K_TXT_KW_RIGHT'..equ'290   DTR$$K_TXT_KW_ROLLBACK'..equ'291 DTR$$K_TXT_KW_RPAREN'..equ'292 DTR$$K_TXT_KW_RSE'..equ'293  DTR$$K_TXT_KW_RUNNING'..equ'294  DTR$$K_TXT_KW_SCALE'..equ'295  DTR$$K_TXT_KW_SCHEMA'..equ'296 DTR$$K_TXT_KW_SCHEMAS'..equ'297  DTR$$K_TXT_KW_SEARCH'..equ'298 DTR$$K_TXT_KW_SECTRLT'..equ'299  DTR$$K_TXT_KW_SELECT'..equ'300 DTR$$K_TXT_KW_SEMI'..equ'301 DTR$$K_TXT_KW_SEMICOL'..equ'302  DTR$$K_TXT_KW_SEND'..equ E   '303  DTR$$K_TXT_KW_SEPARATE'..equ'304 DTR$$K_TXT_KW_SET'..equ'305  DTR$$K_TXT_KW_SETS'..equ'306 DTR$$K_TXT_KW_SETUP'..equ'307  DTR$$K_TXT_KW_SHARED'..equ'308 DTR$$K_TXT_KW_SHOW'..equ'309 DTR$$K_TXT_KW_SHOWP'..equ'310  DTR$$K_TXT_KW_SIGN'..equ'311 DTR$$K_TXT_KW_SIGNED'..equ'312 DTR$$K_TXT_KW_SIZE'..equ'313 DTR$$K_TXT_KW_SKIP'..equ'314 DTR$$K_TXT_KW_SLASH'..equ'315   DTR$$K_TXT_KW_SNAPSHOT'..equ'316 DTR$$K_TXT_KW_SORT'..equ'317 DTR$$K_TXT_KW_SORTED'..equ'318 DTR$$K_TXT_KW_SOURCE'..equ'319 F    DTR$$K_TXT_KW_SPACE'..equ'320  DTR$$K_TXT_KW_STAR'..equ'321  DTR$$K_TXT_KW_STARTING'..equ'322 DTR$$K_TXT_KW_STDDEV'..equ'323 DTR$$K_TXT_KW_STORE'..equ'324  DTR$$K_TXT_KW_STRING'..equ'325 DTR$$K_TXT_KW_STRUCT'..equ'326  DTR$$K_TXT_KW_SUBSCHEM'..equ'327 DTR$$K_TXT_KW_SUM'..equ'328   DTR$$K_TXT_KW_SUPERCED'..equ'329  DTR$$K_TXT_KW_SUPERSED'..equ'330 DTR$$K_TXT_KW_SYNC'..equ'331 DTR$$K_TXT_KW_SYNCHRO'..equ'332  DTR$$K_TXT_KW_SYNONYM'..equ'333   DTR$$K_TXT_KW_SYNONYMS'..equ'334 DTR$$K_TXT_KW_TA G   B'..equ'335  DTR$$K_TXT_KW_TABLE'..equ'336  DTR$$K_TXT_KW_TABLES'..equ'337  DTR$$K_TXT_KW_TERMINAL'..equ'338 DTR$$K_TXT_KW_TEXT'..equ'339 DTR$$K_TXT_KW_THE'..equ'340  DTR$$K_TXT_KW_THEN'..equ'341 DTR$$K_TXT_KW_TIMES'..equ'342  DTR$$K_TXT_KW_TO'..equ'343 DTR$$K_TXT_KW_TOP'..equ'344  DTR$$K_TXT_KW_TOTAL'..equ'345   DTR$$K_TXT_KW_TRAILING'..equ'346 DTR$$K_TXT_KW_UIC'..equ'347  DTR$$K_TXT_KW_UNDERLN'..equ'348   DTR$$K_TXT_KW_UNSIGNED'..equ'349 DTR$$K_TXT_KW_USAGE'..equ'350  DTR$$K_TXT_KW_USER H   '..equ'351 DTR$$K_TXT_KW_USING'..equ'352  DTR$$K_TXT_KW_VALID'..equ'353  DTR$$K_TXT_KW_VALUE'..equ'354   DTR$$K_TXT_KW_VARIABLS'..equ'355 DTR$$K_TXT_KW_VARYING'..equ'356  DTR$$K_TXT_KW_VECTOR'..equ'357 DTR$$K_TXT_KW_VERIFY'..equ'358 DTR$$K_TXT_KW_VERTBAR'..equ'359  DTR$$K_TXT_KW_VIA'..equ'360  DTR$$K_TXT_KW_WHEN'..equ'361 DTR$$K_TXT_KW_WHILE'..equ'362  DTR$$K_TXT_KW_WITH'..equ'363  DTR$$K_TXT_KW_WITHFORM'..equ'364 DTR$$K_TXT_KW_WITHIN'..equ'365 DTR$$K_TXT_KW_WORD'..equ'366 DTR$$K_TXT_KW_ I   WRITE'..equ'367  DTR$$K_TXT_KW_ZERO'..equ'368 DTR$$K_TXT_KW_ZONED'..equ'369  DTR$$K_TXT_KW_SFLOAT'..equ'370 DTR$$K_TXT_KW_TFLOAT'..equ'371 DTR$$K_TXT_KW_WIDTH'..equ'372  DTR$$K_TXT_KW_SPARE1'..equ'373 DTR$$K_TXT_KW_SPARE2'..equ'374 DTR$$K_TXT_KW_SPARE3'..equ'375 DTR$$K_TXT_KW_SPARE4'..equ'376 DTR$$K_TXT_KW_SPARE5'..equ'377 DTR$$K_TXT_KW_SPARE6'..equ'378 DTR$$K_TXT_KW_SPARE7'..equ'379 DTR$$K_TXT_KW_SPARE8'..equ'380 DTR$$K_TXT_KW_SPARE9'..equ'381  DTR$$K_TXT_MSC_CURRENT'..equ'382! DTR$$K_T J   XT_MSC_FLOATPIC'..equ'383 ! DTR$$K_TXT_MSC_STDEVPIC'..equ'384   DTR$$K_TXT_MSC_RDBNULL'..equ'385 DTR$$K_TXT_MSC_SPARE0'..equ'386  DTR$$K_TXT_MSC_SPARE1'..equ'387  DTR$$K_TXT_MSC_SPARE2'..equ'388  DTR$$K_TXT_MSC_SPARE3'..equ'389  DTR$$K_TXT_MSC_SPARE4'..equ'390  DTR$$K_TXT_MSC_SPARE5'..equ'391  DTR$$K_TXT_MSC_SPARE6'..equ'392  DTR$$K_TXT_MSC_SPARE7'..equ'393  DTR$$K_TXT_MSC_SPARE8'..equ'394  DTR$$K_TXT_MSC_SPARE9'..equ'395 ! DTR$$K_TXT_PRM_ALLOCRUL'..equ'396   DTR$$K_TXT_PRM_ARGLIST'..equ'397 K    DTR$$K_TXT_PRM_AS'..equ'398  DTR$$K_TXT_PRM_BOOEXP'..equ'399 ! DTR$$K_TXT_PRM_COLLNAME'..equ'400  DTR$$K_TXT_PRM_COLON'..equ'401  DTR$$K_TXT_PRM_COLPAGE'..equ'402 DTR$$K_TXT_PRM_COMMA'..equ'403! DTR$$K_TXT_PRM_CONSTANT'..equ'404 ! DTR$$K_TXT_PRM_DATATYPE'..equ'405  DTR$$K_TXT_PRM_DATE'..equ'406 ! DTR$$K_TXT_PRM_DATORNUM'..equ'407   DTR$$K_TXT_PRM_DECLARE'..equ'408! DTR$$K_TXT_PRM_DEFINOBJ'..equ'409 ! DTR$$K_TXT_PRM_DEFINPSU'..equ'410 ! DTR$$K_TXT_PRM_DEFINSUB'..equ'411   DTR$$K_TXT_PRM_DFNPOPT'. L   .equ'412  DTR$$K_TXT_PRM_DFNPSEQ'..equ'413! DTR$$K_TXT_PRM_DICTPATH'..equ'414   DTR$$K_TXT_PRM_DOMNAME'..equ'415 DTR$$K_TXT_PRM_END'..equ'416  DTR$$K_TXT_PRM_ENDCASE'..equ'417 DTR$$K_TXT_PRM_ENDCOM'..equ'418   DTR$$K_TXT_PRM_ENDPLOT'..equ'419! DTR$$K_TXT_PRM_ENDSTATE'..equ'420 ! DTR$$K_TXT_PRM_ENDTABLE'..equ'421  DTR$$K_TXT_PRM_EQUALS'..equ'422 ! DTR$$K_TXT_PRM_FIELDLST'..equ'423 ! DTR$$K_TXT_PRM_FIELDNAM'..equ'424 ! DTR$$K_TXT_PRM_FIELDOPT'..equ'425 ! DTR$$K_TXT_PRM_FILENAME'..equ'426  DTR$$K_TXT M   _PRM_FILEOP'..equ'427 ! DTR$$K_TXT_PRM_FORMNAME'..equ'428  DTR$$K_TXT_PRM_FRMREC'..equ'429 ! DTR$$K_TXT_PRM_FRMRECLS'..equ'430  DTR$$K_TXT_PRM_FROM'..equ'431 ! DTR$$K_TXT_PRM_FRORRCLS'..equ'432   DTR$$K_TXT_PRM_HEADSEG'..equ'433 DTR$$K_TXT_PRM_HELP'..equ'434  DTR$$K_TXT_PRM_IN'..equ'435  DTR$$K_TXT_PRM_KEYOPT'..equ'436   DTR$$K_TXT_PRM_KEYWORD'..equ'437! DTR$$K_TXT_PRM_LEADTRAL'..equ'438 ! DTR$$K_TXT_PRM_LINEPAGE'..equ'439  DTR$$K_TXT_PRM_LPAREN'..equ'440 ! DTR$$K_TXT_PRM_MAXLINES'..equ'441 ! DT N   R$$K_TXT_PRM_NODESPEC'..equ'442   DTR$$K_TXT_PRM_NOTDVAR'..equ'443 DTR$$K_TXT_PRM_NUMBER'..equ'444 ! DTR$$K_TXT_PRM_NXTLISEL'..equ'445   DTR$$K_TXT_PRM_OBJNAME'..equ'446  DTR$$K_TXT_PRM_OCCFROM'..equ'447 DTR$$K_TXT_PRM_OCCURS'..equ'448  DTR$$K_TXT_PRM_OF'..equ'449  DTR$$K_TXT_PRM_ON'..equ'450  DTR$$K_TXT_PRM_PAGE'..equ'451  DTR$$K_TXT_PRM_PERIOD'..equ'452  DTR$$K_TXT_PRM_PICREP'..equ'453   DTR$$K_TXT_PRM_PICTURE'..equ'454! DTR$$K_TXT_PRM_PLOTNAME'..equ'455  DTR$$K_TXT_PRM_PRINT'..equ'456! DTR$ O   $K_TXT_PRM_PROJLIST'..equ'457 ! DTR$$K_TXT_PRM_QUERNAME'..equ'458 ! DTR$$K_TXT_PRM_REDEFSUB'..equ'459   DTR$$K_TXT_PRM_RELATOP'..equ'460! DTR$$K_TXT_PRM_RELSHIPS'..equ'461   DTR$$K_TXT_PRM_REPNAME'..equ'462 DTR$$K_TXT_PRM_REPORT'..equ'463 ! DTR$$K_TXT_PRM_REPSTATE'..equ'464  DTR$$K_TXT_PRM_RPAREN'..equ'465   DTR$$K_TXT_PRM_RPFNAME'..equ'466 DTR$$K_TXT_PRM_RSE'..equ'467 DTR$$K_TXT_PRM_SCHEMA'..equ'468   DTR$$K_TXT_PRM_SETLIST'..equ'469  DTR$$K_TXT_PRM_SETNAME'..equ'470 DTR$$K_TXT_PRM_SETNUM'..equ' P   471  DTR$$K_TXT_PRM_SETOPT'..equ'472 ! DTR$$K_TXT_PRM_SETVALUE'..equ'473   DTR$$K_TXT_PRM_SHOWOPT'..equ'474! DTR$$K_TXT_PRM_SNDORRCV'..equ'475 ! DTR$$K_TXT_PRM_SORTLIST'..equ'476 ! DTR$$K_TXT_PRM_SOURCLAU'..equ'477 ! DTR$$K_TXT_PRM_STATMENT'..equ'478 ! DTR$$K_TXT_PRM_STATOREN'..equ'479 ! DTR$$K_TXT_PRM_SUBSCHEM'..equ'480   DTR$$K_TXT_PRM_SYNONYM'..equ'481! DTR$$K_TXT_PRM_TERMQUOT'..equ'482 ! DTR$$K_TXT_PRM_TIMEORTO'..equ'483  DTR$$K_TXT_PRM_TO'..equ'484 ! DTR$$K_TXT_PRM_TOOITEMS'..equ'485 ! DTR$$K_T�                                                                                                                                                                                                                                                   &                        ���� $      
DTRJA061.C                     �  
  [C]DTRTXTLIB.MLB;1                                                                                                                  ]                         �F      P        Q   XT_PRM_TOPORBOT'..equ'486   DTR$$K_TXT_PRM_TOTCOUN'..equ'487 DTR$$K_TXT_PRM_USAGE'..equ'488! DTR$$K_TXT_PRM_USEORRET'..equ'489 ! DTR$$K_TXT_PRM_USERNAME'..equ'490  DTR$$K_TXT_PRM_USING'..equ'491 DTR$$K_TXT_PRM_VALUEX'..equ'492  DTR$$K_TXT_PRM_ZERO'..equ'493  DTR$$K_TXT_PRM_DOCFMT'..equ'494   DTR$$K_TXT_PRM_ATTNAME'..equ'495 DTR$$K_TXT_PRM_ATTOPT'..equ'496   DTR$$K_TXT_PRM_RENDOPT'..equ'497 DTR$$K_TXT_PRM_ATTVAL'..equ'498 ! DTR$$K_TXT_PRM_FIECONST'..equ'499  DTR$$K_TXT_PRM_UNIT'..equ'500   DTR$ R   $K_TXT_PRM_NUMPRMP'..equ'501 DTR$$K_TXT_PRM_QUOSTR'..equ'502  DTR$$K_TXT_PRM_ORIENT'..equ'503  DTR$$K_TXT_PRM_SPARE0'..equ'504  DTR$$K_TXT_PRM_SPARE1'..equ'505  DTR$$K_TXT_PRM_SPARE2'..equ'506  DTR$$K_TXT_PRM_SPARE3'..equ'507  DTR$$K_TXT_PRM_SPARE4'..equ'508  DTR$$K_TXT_PRM_SPARE5'..equ'509  DTR$$K_TXT_PRM_SPARE6'..equ'510  DTR$$K_TXT_PRM_SPARE7'..equ'511  DTR$$K_TXT_PRM_SPARE8'..equ'512  DTR$$K_TXT_PRM_SPARE9'..equ'513   DTR$$K_TXT_REP_AVERAGE'..equ'514 DTR$$K_TXT_REP_COUNT'..equ'515 DT S   R$$K_TXT_REP_MAX'..equ'516 DTR$$K_TXT_REP_MIN'..equ'517 DTR$$K_TXT_REP_PAGE'..equ'518 ! DTR$$K_TXT_REP_RUNCOUNT'..equ'519  DTR$$K_TXT_REP_RUNTOT'..equ'520 ! DTR$$K_TXT_REP_STANDDEV'..equ'521  DTR$$K_TXT_REP_TOTAL'..equ'522  DTR$$K_TXT_REP_NEWPAGE'..equ'523  DTR$$K_TXT_REP_NEWSECT'..equ'524 DTR$$K_TXT_REP_DETAIL'..equ'525  DTR$$K_TXT_REP_TOPOF'..equ'526 DTR$$K_TXT_REP_BOTOF'..equ'527" DTR$$K_TXT_REP_BREAKPAGE'..equ'528 DTR$$K_TXT_REP_REPORT'..equ'529 ! DTR$$K_TXT_REP_FUNCTION'..equ'530  DTR$$K T   _TXT_REP_CHOICE'..equ'531 ! DTR$$K_TXT_REP_CONSTANT'..equ'532  DTR$$K_TXT_REP_EXPR'..equ'533  DTR$$K_TXT_REP_STRING'..equ'534  DTR$$K_TXT_REP_PROMPT'..equ'535  DTR$$K_TXT_REP_SPARE1'..equ'536  DTR$$K_TXT_REP_SPARE2'..equ'537  DTR$$K_TXT_REP_SPARE3'..equ'538  DTR$$K_TXT_REP_SPARE4'..equ'539  DTR$$K_TXT_REP_SPARE5'..equ'540  DTR$$K_TXT_REP_SPARE6'..equ'541  DTR$$K_TXT_REP_SPARE7'..equ'542  DTR$$K_TXT_REP_SPARE8'..equ'543  DTR$$K_TXT_REP_SPARE9'..equ'544  DTR$$K_TXT_SHO_ACCEXT'..equ'545  DT U   R$$K_TXT_SHO_ACCMOD'..equ'546   DTR$$K_TXT_SHO_ACCREAD'..equ'547! DTR$$K_TXT_SHO_ACCWRITE'..equ'548   DTR$$K_TXT_SHO_CONCURR'..equ'549  DTR$$K_TXT_SHO_CONSIST'..equ'550 DTR$$K_TXT_SHO_DICDOM'..equ'551   DTR$$K_TXT_SHO_DICPROC'..equ'552 DTR$$K_TXT_SHO_DICREC'..equ'553  DTR$$K_TXT_SHO_DICTAB'..equ'554 ! DTR$$K_TXT_SHO_DTYPCOMP'..equ'555 ! DTR$$K_TXT_SHO_DTYPDATE'..equ'556   DTR$$K_TXT_SHO_DTYPIDX'..equ'557! DTR$$K_TXT_SHO_DTYPIXAE'..equ'558 ! DTR$$K_TXT_SHO_DTYPIXAG'..equ'559 ! DTR$$K_TXT_SHO_DTYPIXP V   E'..equ'560 ! DTR$$K_TXT_SHO_DTYPIXPG'..equ'561 ! DTR$$K_TXT_SHO_DTYPLIST'..equ'562   DTR$$K_TXT_SHO_DTYPNBR'..equ'563  DTR$$K_TXT_SHO_DTYPSEG'..equ'564  DTR$$K_TXT_SHO_DTYPSTR'..equ'565! DTR$$K_TXT_SHO_DTYPUNDE'..equ'566 ! DTR$$K_TXT_SHO_DTYPXFLD'..equ'567  DTR$$K_TXT_SHO_OBJDB'..equ'568! DTR$$K_TXT_SHO_OBJDBREC'..equ'569 ! DTR$$K_TXT_SHO_OBJDITAB'..equ'570  DTR$$K_TXT_SHO_OBJDOM'..equ'571   DTR$$K_TXT_SHO_OBJDTAB'..equ'572 DTR$$K_TXT_SHO_OBJREL'..equ'573   DTR$$K_TXT_SHO_PROTCMD'..equ'574  DTR$$K W   _TXT_SHO_PROTEXT'..equ'575! DTR$$K_TXT_SHO_PROTFORW'..equ'576 ! DTR$$K_TXT_SHO_PROTGDEL'..equ'577 ! DTR$$K_TXT_SHO_PROTHEAD'..equ'578 ! DTR$$K_TXT_SHO_PROTHIST'..equ'579 ! DTR$$K_TXT_SHO_PROTLDEL'..equ'580   DTR$$K_TXT_SHO_PROTMOD'..equ'581! DTR$$K_TXT_SHO_PROTNACC'..equ'582 ! DTR$$K_TXT_SHO_PROTPASS'..equ'583 ! DTR$$K_TXT_SHO_PROTREAD'..equ'584   DTR$$K_TXT_SHO_PROTSEE'..equ'585! DTR$$K_TXT_SHO_PROTUPDA'..equ'586 ! DTR$$K_TXT_SHO_PROTWRIT'..equ'587 ! DTR$$K_TXT_SHO_PROTXTND'..equ'588  DTR$$K_TXT_SHO X   _RMSIDX'..equ'589  DTR$$K_TXT_SHO_RMSREL'..equ'590  DTR$$K_TXT_SHO_RMSSEQ'..equ'591   DTR$$K_TXT_SHO_SEGMENT'..equ'592  DTR$$K_TXT_SHO_SETAUTO'..equ'593 DTR$$K_TXT_SHO_SETFIX'..equ'594  DTR$$K_TXT_SHO_SETMAN'..equ'595   DTR$$K_TXT_SHO_SETMAND'..equ'596 DTR$$K_TXT_SHO_SETMEM'..equ'597  DTR$$K_TXT_SHO_SETNAM'..equ'598  DTR$$K_TXT_SHO_SETOPT'..equ'599  DTR$$K_TXT_SHO_SETOWN'..equ'600   DTR$$K_TXT_SHO_SHARDEF'..equ'601  DTR$$K_TXT_SHO_SHAREXC'..equ'602! DTR$$K_TXT_SHO_SHARPROT'..equ'603 ! DTR$$K_T Y   XT_SHO_SHARSHAR'..equ'604 ! DTR$$K_TXT_SHO_SHARSNAP'..equ'605 ! DTR$$K_TXT_SHO_TYPEDBMS'..equ'606 ! DTR$$K_TXT_SHO_TYPEPORT'..equ'607   DTR$$K_TXT_SHO_TYPERDB'..equ'608  DTR$$K_TXT_SHO_TYPEREM'..equ'609  DTR$$K_TXT_SHO_TYPERMS'..equ'610! DTR$$K_TXT_SHO_TYPEVIEW'..equ'611  DTR$$K_TXT_SHO_SPARE0'..equ'612  DTR$$K_TXT_SHO_SPARE1'..equ'613  DTR$$K_TXT_SHO_SPARE2'..equ'614  DTR$$K_TXT_SHO_SPARE3'..equ'615  DTR$$K_TXT_SHO_SPARE4'..equ'616  DTR$$K_TXT_SHO_SPARE5'..equ'617  DTR$$K_TXT_SHO_SPARE6'..equ' Z   618  DTR$$K_TXT_SHO_SPARE7'..equ'619  DTR$$K_TXT_SHO_SPARE8'..equ'620  DTR$$K_TXT_SHO_SPARE9'..equ'621  DTR$$K_TXT_UDK_ADT'..equ'622 DTR$$K_TXT_UDK_AT'..equ'623  DTR$$K_TXT_UDK_CLOSE'..equ'624 DTR$$K_TXT_UDK_EDIT'..equ'625 ! DTR$$K_TXT_UDK_EDITBACK'..equ'626 ! DTR$$K_TXT_UDK_EDITSHOW'..equ'627  DTR$$K_TXT_UDK_EXIT'..equ'628  DTR$$K_TXT_UDK_GUIDE'..equ'629  DTR$$K_TXT_UDK_HELINES'..equ'630 DTR$$K_TXT_UDK_HELP'..equ'631  DTR$$K_TXT_UDK_HELPQ'..equ'632! DTR$$K_TXT_UDK_HELPSHOW'..equ'633 ! DTR$     $K_TXT_UDK_HEPROMPT'..equ'634 ! DTR$$K_TXT_UDK_HEWINDOW'..equ'635  DTR$$K_TXT_UDK_OPEN'..equ'636  DTR$$K_TXT_UDK_SPARE0'..equ'637  DTR$$K_TXT_UDK_SPARE1'..equ'638  DTR$$K_TXT_UDK_SPARE2'..equ'639  DTR$$K_TXT_UDK_SPARE3'..equ'640  DTR$$K_TXT_UDK_SPARE4'..equ'641  DTR$$K_TXT_UDK_SPARE5'..equ'642  DTR$$K_TXT_UDK_SPARE6'..equ'643  DTR$$K_TXT_UDK_SPARE7'..equ'644  DTR$$K_TXT_UDK_SPARE8'..equ'645  DTR$$K_TXT_UDK_SPARE9'..equ'646  DTR$$K_TXT_TOTAL'..equ'647 	.ENDM w w                                 ��J �d�l�*�  HAYASE      $DTR$TEXT_INIT$DTR$TEXT_ENTRY$DTR$TXT_CONSTANTS                                                                                                                                                                                                                                                                                                                                                                                                                                            �               * [C]ENTRY.COB;1 +  , !   .     / s P   4 O       H                    - 
    0  � 1    2   3      K � P   W   O     5   6 @9���"�  7  �R*�*�  8          9          G s P  H �� J �                M ***************************************************************************** M * To run this program, you must compile it and link it with the DATATRIEVE  * M * shareable image:                                                          * M *                                                                           * M *  $ COBOL ENTRY                                                            * M *  $ LINK ENTRY, DTR/OPT                                                    * M *  $ RUN ENTRY                                                              * M *                                                                           * M * See the VAX DATATRIEVE Guide to Programming and Customizing for more      * M * information on using an option file to link programs that call DATATRIEVE.* M *****************************************************************************  * H * The program ENTRY accepts data entered by a user from the terminal andF * stores the data in the DATATRIEVE domain PERSONNEL. The program uses< * Run-Time Library procedures to format the terminal screen. * N * The program uses a port called PERSONNEL_PORT to pass records to DATATRIEVE.N * Because data is entered from the terminal in ASCII format, all fields in theN * port are declared as character data types. Here are the DATATRIEVE commands $ * to define the port PERSONNEL_PORT: * 7 * DEFINE PORT PERSONNEL_PORT USING PERSONNEL_ENTRY_REC;  * ) * DEFINE RECORD PERSONNEL_ENTRY_REC USING  * 01  PERSON. ( *     05 ID                   PIC  X(5).) *     05 EMPLOYEE_STATUS      PIC  X(11).  *     05 EMPLOYEE_NAME. ) *         10 FIRST_NAME       PIC  X(10). ) *         10 LAST_NAME        PIC  X(10). ' *     05 DEPT                 PIC  XXX. ) *     05 START_DATE           PIC  X(11). ( *     05 SALARY               PIC  X(5).( *     05 SUP_ID               PIC  X(5). *; * O *******************************************************************************  IDENTIFICATION DIVISION. PROGRAM-ID.            ENTRY. 8 ****************************�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  '                        �&' $      
DTRJA061.C                     !  
  [C]ENTRY.COB;1                                                                                                                 O                              �� "            **************************** ENVIRONMENT DIVISION.  CONFIGURATION SECTION. DATA DIVISION. WORKING-STORAGE SECTION.  9 ********************************************************* 0 * Copy in the DATATRIEVE Access Block		        *9 *********************************************************    	COPY "DTR$LIBRARY:DAB.LIB".  6 ******************************************************6 * Set up a record buffer by copying the CDD record   *6 * definition.                                        *6 ******************************************************  , 	COPY "PERSONNEL_ENTRY_REC" FROM DICTIONARY.  6 ******************************************************6 * Declare screen formatting variables.               *6 ******************************************************  " 01 LINE_POS        PIC S9(9) COMP." 01 COL_POS         PIC S9(9) COMP.* 01 BOLD_VIDEO      PIC S9(9) COMP VALUE 1.* 01 REVERSE_VIDEO   PIC S9(9) COMP VALUE 2. 01 TXT             PIC X(50). ' 01 SPACE_TXT       PIC X     VALUE " ". 6 01 HDR_TXT         PIC X(16) VALUE "PERSONNEL RECORD".6 01 FNAME_TXT       PIC X(16) VALUE "NAME: __________".0 01 LNAME_TXT       PIC X(10) VALUE "__________".9 01 STAT_TXT        PIC X(19) VALUE "STATUS: ___________". / 01 ID_TXT          PIC X(9)  VALUE "ID: _____". 5 01 DEPT_TXT        PIC X(15) VALUE "DEPARTMENT: ___". : 01 SUP_TXT         PIC X(20) VALUE "SUPERVISOR ID: _____".> 01 DATE_TXT        PIC X(24) VALUE "START DATE: ____________".3 01 SAL_TXT         PIC X(13) VALUE "SALARY: _____".   $ 01 STACK_SIZE PIC 99 COMP VALUE 100. 01 DTR_OPTIONS PIC 9(9) COMP.  01 CONT PIC X. 01 COMMAND_LINE PIC X(80) <        VALUE "READY PERSONNEL WRITE; READY PERSONNEL_PORT;".   PROCEDURE DIVISION.    010-INITIALIZE-INTERFACE.   A ***************************************************************** A * Initialize the interface with DTR$INIT.  Use DTR$COMMAND to   * A * ready domain and port.                                        * A *****************************************************************   % 	CALL "DTR$INIT" USING DAB STACK-SIZE # 			BY DESCRIPTOR MSG_BUFF AUX_BUFF.    	CALL "DTR$COMMAND" USING DAB  			BY DESCRIPTOR COMMAND_LINE.  A ***************************************************************** A * Use DTR$DTR to display messages and return control on the     * A * DTR$K_STL_CMD stallpoint.                                     * A *****************************************************************   ( 	CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.  A ***************************************************************** A * Command DATATRIEVE to store PERSONNEL records from the port.  * A * DATATRIEVE stalls at DTR$K_STL_PPUT                           * A *****************************************************************    	MOVE SPACES TO COMMAND_LINE. A 	MOVE "FOR PERSONNEL_PORT STORE PERSONNEL USING PERSON = PERSON;"  		TO COMMAND_LINE. 	CALL "DTR$COMMAND" USING DAB  			BY DESCRIPTOR COMMAND_LINE.   100-STORE-A-RECORD.   ? *************************************************************** ? * Read in a data record.                                      * ? ***************************************************************  	PERFORM 200-GET-A-RECORD.  @ ****************************************************************@ * Use the call DTR$PUT_PORT to pass the record to DATATRIEVE.  *@ ****************************************************************  & 	CALL "DTR$PUT_PORT" USING DAB PERSON.  C ******************************************************************* C * Use the DTR$DTR call to display messages. Return control to the * C * program on DTR$K_STL_CMD and DTR$K_STL_PPUT.                    * C *******************************************************************   5 	ADD DTR$M_OPT_CMD DTR$M_OPT_PPUT GIVING DTR_OPTIONS.  	IF DTR$K_STL_MSG THEN  ' 		CALL "DTR$DTR" USING DAB DTR_OPTIONS. : 	IF NOT DTR$K_STL_PPUT THEN PERFORM 300-RECORD-NOT-STORED  		UNTIL DTR$K_STL_PPUT.  	GO TO 100-STORE-A-RECORD.   200-GET-A-RECORD.    ********************** * Clear the screen.  * ********************** 	MOVE 1 TO LINE_POS. 	MOVE 1 TO COL_POS. . 	CALL "LIB$ERASE_PAGE" USING LINE_POS COL_POS.   ********************** * Format the screen. * **********************  7 	MOVE "Enter a carriage return after each field value." 	 		TO TXT.  	MOVE 1 TO LINE_POS. 	MOVE 6 TO COL_POS. . 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR TXT! 			BY REFERENCE LINE_POS COL_POS.    	MOVE SPACES TO TXT./ 	MOVE "To stop storing records enter ALL DONE." 	 		TO TXT.  	MOVE 3 TO LINE_POS. 	MOVE 8 TO COL_POS. . 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR TXT  		BY REFERENCE LINE_POS COL_POS.   	MOVE 6 TO LINE_POS. 	MOVE 25 TO COL_POS.2 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR HDR_TXT+ 		BY REFERENCE LINE_POS COL_POS BOLD_VIDEO.    	MOVE 9 TO LINE_POS. 	MOVE 10 TO COL_POS.4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR FNAME_TXT/ 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.  	MOVE 9 TO LINE_POS. 	MOVE 27 TO COL_POS. 	MOVE "          " TO TXT.4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR LNAME_TXT/ 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    	MOVE 40 TO COL_POS. 	MOVE "(FIRST LAST)" TO TXT.. 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR TXT! 			BY REFERENCE LINE_POS COL_POS.  	MOVE 11 TO LINE_POS.  	MOVE 8 TO COL_POS. 3 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR STAT_TXT / 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    	MOVE 40 TO COL_POS.( 	MOVE "(TRAINEE or EXPERIENCED)" TO TXT.. 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR TXT! 			BY REFERENCE LINE_POS COL_POS.    	MOVE 12 TO LINE_POS.  	MOVE 12 TO COL_POS.1 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR ID_TXT / 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    	MOVE 13 TO LINE_POS.  	MOVE 4 TO COL_POS. 3 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR DEPT_TXT / 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    	MOVE 14 TO LINE_POS.  	MOVE 1 TO COL_POS. 2 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SUP_TXT/ 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    	MOVE 15 TO LINE_POS.  	MOVE 4 TO COL_POS. 3 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR DATE_TXT / 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.  	MOVE 40 TO COL_POS.& 	MOVE "(EXAMPLE: 01-Jan-1982)" TO TXT.. 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR TXT! 			BY REFERENCE LINE_POS COL_POS.    	MOVE 16 TO LINE_POS.  	MOVE 8 TO COL_POS. 2 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SAL_TXT/ 			BY REFERENCE LINE_POS COL_POS REVERSE_VIDEO.    *****************  * Accept input. *  *****************    	MOVE 9 TO LINE_POS. 	MOVE 16 TO COL_POS.4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT FIRST_NAME. / 	IF FIRST_NAME = "ALL DONE" THEN GO TO 999-EOJ.    	MOVE 27 TO COL_POS.4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT LAST_NAME.   	MOVE 11 TO LINE_POS.  	MOVE 16 TO COL_POS.4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT EMPLOYEE_STATUS.   	MOVE 12 TO LINE_POS. 4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT ID.    	MOVE 13 TO LINE_POS. 4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT DEPT.    	MOVE 14 TO LINE_POS. 4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT SUP_ID.    	MOVE 15 TO LINE_POS. 4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT START_DATE.    	MOVE 16 TO LINE_POS. 4 	CALL "LIB$PUT_SCREEN" USING BY DESCRIPTOR SPACE_TXT! 			BY REFERENCE LINE_POS COL_POS.  	ACCEPT SALARY.    300-RECORD-NOT-STORED.  " 	DISPLAY "Record was not stored.".% 	DISPLAY "Press RETURN to continue.".  	ACCEPT CONT.  	CALL "DTR$COMMAND" USING DAB % 		        BY DESCRIPTOR COMMAND_LINE.    999-EOJ. **************************** * User entered ALL DONE.   * ****************************   	MOVE 1 TO LINE_POS. 	MOVE 1 TO COL_POS. . 	CALL "LIB$ERASE_PAGE" USING LINE_POS COL_POS.   **************************** * Stop storing records.    * ****************************   	CALL "DTR$PORT_EOF" USING DAB.    	CALL "DTR$FINISH" USING DAB.* 	DISPLAY "End of program.".*
 	STOP RUN.��***** M * To run this program, you must compile it and link it with the DATATRIEVE  * M * shareable image:                                                          * M *                                                                           * M *  $ COBOL ENTRY                                                            * M *  $ LINK ENTRY, DTR/OPT                                                    * M *  $ RUN ENTRY                �                                                                                                                                                                                                                                                   (                        P�1� $      
DTRJA061.C                     R  
  [C]ESTABLISH.FOR;1                                                                                                             O     	                         ���               * [C]ESTABLISH.FOR;1 +  , R   . 	    / s P   4 O   	   	 �                   - 
    0  � 1    2   3      K � P   W   O 
    5   6  ���"�  7 �c*�*�  8          9          G s P  H �� J �                            K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C  C > C*************************************************************0 C                   SUBROUTINE ESTABLISH		     *> C Searches the current domain for records that fit the       *> C description given to DATATRIEVE and forms a collection.    *> C This program is a subroutine of the MENU.FOR program       *> C*************************************************************   	SUBROUTINE ESTABLISH (DOMAIN) 	INCLUDE 'DTR$LIBRARY:DAB' 	CHARACTER*31 ATTRIBUTE 	  	CHARACTER*31 ANSWER 	CHARACTER*31 BOOLEAN  	CHARACTER*31 DOMAIN 	CHARACTER*31 VALUE  	INTEGER*4    NUM_RECS 	INTEGER*2    LINE/1/, COL/1/    C Clear the screen.     	CALL LIB$ERASE_PAGE (LINE, COL)  > C Ask if the user wishes to use all the records in the domain.@ C If the response is yes, then issue the FIND statement for the  C entire domain.   	WRITE (6, 101) 0 101	FORMAT (' Do you wish to use all the records 	1 in the domain? ',$) 	READ (5, 102)ANSWER 102	FORMAT (A): 	IF (ANSWER(1:1) .EQ. 'Y' .OR. ANSWER(1:1) .EQ. 'y')  THEN, 		CALL DTR$COMMAND (DAB,'FIND !CMD;',DOMAIN) 		CALL MESSAGE 		RETURN 	END IF    G C Show the user the fields available for a record selection expression.   6 105	CALL DTR$COMMAND (DAB,'SHOW FIELDS !CMD;', DOMAIN)  @ C Call the terminal server to handle the printing of the fields.8 C Return to the program on the DTR$K_STL_CMD stallpoint.  " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  G C Prompt the user for a field name, a relational operator, and a value.    	WRITE (6, 110) < 110	FORMAT (' The collection will be formed on the basis of': 	1 / ' identifying characteristics you choose. You should'9 	2 / ' enter a FIELD, an EQUATION, and an ATTRIBUTE. For' : 	3 / ' example, if your domain is EMPLOYEES, you can form' 	4 / ' a collection of:') 	5 / ' EMPLOYEES whose SALARY (FIELD) is' & 	6 / ' GT (RELATION) $25,000 (VALUE).'9 	7 ///' Enter the FIELD (SALARY, PRICE, DEPARTMENT): ',$)  	READ (5,102) ATTRIBUTE    	WRITE (6, 115) D 115	FORMAT (' Enter the RELATION ( EQ, GT, GE, LT, CONTAINING): ',$) 	READ (5, 102) BOOLEAN   	WRITE (6, 120) > 120	FORMAT (' Enter the VALUE (non-numeric values should be in 	1 quotes): ' ,$)  	READ (5, 102) VALUE  2 C Instruct DATATRIEVE to find the desired records.   	CALL DTR$COMMAND - 	1  (DAB,'FIND CURRENT WITH !CMD !CMD !CMD;',  	2   ATTRIBUTE, BOOLEAN, VALUE)   1 C Call the subroutine MESSAGE to handle messages.    	CALL MESSAGE   2 C Verify that the FIND was completed successfully.  2 	IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN 		GO TO 105  	END IF   ' C Verify that there were records found. 8 C If no records were found, the user must either use all: C records in the collection or establish a new collection.  8 	CALL DTR$COMMAND (DAB, 'STORE PT1 USING NUM = COUNT;')	   C Display messages.   ) 	IF (DAB$W_STATE .EQ. DTR$K_STL_MSG) THEN  		CALL MESSAGE 	END IF   @ C Use DTR$GET_PORT call to retrieve the number of records found.  * 	IF (DAB$W_STATE .EQ. DTR$K_STL_PGET) THEN# 		CALL DTR$GET_PORT (DAB, NUM_RECS) # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	 	END IF 	   = C If no records were found, notify the user and find all the   C records in the domain.  9 C Prompt the user to continue or return to the main menu.    	IF (NUM_RECS .EQ. 0) THEN- 		CALL DTR$COMMAND (DAB, 'FIND !CMD;',DOMAIN)  		CALL MESSAGE 		WRITE (6, 135)< 135		FORMAT (' There are no records that fit.'/' Do you wish- 	1 to use all the records in the domain? ',$)  		READ (5,102) ANSWER : 		IF (ANSWER(1:1) .EQ. 'Y' .OR. ANSWER(1:1) .EQ. 'y') THEN	 			RETURN  		END IF 		GO TO 105  	END IF   " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  1 C Ask if the user wishes to make a subcollection.    160	WRITE (6, 165)C 165	FORMAT (' Would you like to establish a sub-collection from the  	1 current collection?') 	READ (5,102) ANSWER9 	IF (ANSWER(1:1) .EQ. 'Y' .OR. ANSWER(1:1) .EQ. 'y') THEN  		GO TO 105  	END IF            RETURN 	END��                                                                                                                            �               * [C]EXAMPLE.BAS;1 +  , T   . 	    / s P   4 L   	    >                    - 
    0  � 1    2   3      K � P   W   O     5   6 `l`v �  7 `
v*�*�  8          9          G s P  H �� J �              . 1       ! Include the DATATRIEVE Access Block.  " 100     %INCLUDE "DTR$LIBRARY:DAB"           ! Declare variables.           DECLARE STRING DOMAIN   3         ! Initialize the DATATRIEVE Call Interface.   ; 200     CALL DTR$INIT (DAB BY REF, 100% BY REF, MSG_BUFF, & 6                 AUX_BUFF, DTR$K_SEMI_COLON_OPT BY REF)  ?         ! Use a Run-Time Library procedure to clear the screen.   *         CALL LIB$ERASE_PAGE BY REF (1%,1%)  I         ! Prompt the user to choose a domain and read in the domain name.   4         PRINT "The following domains are available:"         PRINT ""         PRINT "OWNERS"         PRINT "PERSONNEL"          PRINT "YACHTS"   300     PRINT ""=         PRINT "Enter the name of the domain you want to see";          INPUT DOMAIN  I         ! Use the call DTR$COMMAND to pass a READY command to DATATRIEVE. 8         ! Use the substitution directive CMD to pass the         ! selected domain name.   ;         CALL DTR$COMMAND (DAB BY REF, "READY !CMD", DOMAIN)   =         ! Each time DATATRIEVE attempts to execute a command, ?         ! it puts a condition code in the DAB$L_CONDITION field          ! of the DAB.   H         ! If DATATRIEVE successfully executes the READY command, it puts8         ! the condition DTR$_SUCCESS in DAB$L_CONDITION.  G         ! To check for errors, examine the contents of DAB$L_CONDITION.   /         IF DAB$L_CONDITION <> DTR$_SUCCESS THEN   I         ! If the READY command was not completed successfully, DATATRIEVE G         ! stores an error message in the DAB$A_MSG_BUF field and stalls $         ! at the message stallpoint.  ?         ! The call DTR$DTR transfers control to DATATRIEVE. The E         ! DTR$M_OPT_CMD argument to this call instructs DATATRIEVE to H         ! return control to the program when DATATRIEVE is ready for the         ! next command.   J         ! DATATRIEVE displays its error message and returns control to the         ! program.  8                 CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)  4         ! Reprompt the user for a valid domain name.  "                 PRINT "Try again."                 GO TO 300          END IF  H         ! DATATRIEVE has successfully executed the READY command and hasH         ! a message for the program: "Statement successfully completed."  :         ! DATATRIEVE now stalls at the message stallpoint.  J         ! To recover from this stallpoint and continue passing commands to0         ! DATATRIEVE, use the call DTR$CONTINUE.  &         CALL DTR$CONTINUE BY REF (DAB)  =         ! Pass the PRINT domain-name statement to DATATRIEVE.   ;         CALL DTR$COMMAND (DAB BY REF, "PRINT !CMD", DOMAIN)   F         ! DATATRIEVE now has print lines for the program. It is at the          ! print line stallpoint.	         ! L         ! Use the call DTR$DTR to transfer control to DATATRIEVE. DATATRIEVEL         ! displays the record stream and returns control to the program when+         ! it is ready for the next command.   0         CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)  !         ! End the Call Interface.   $         CALL DTR$FINISH BY REF (DAB)           END ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                �               * [C]EXAMPLE.C;1 +  , V   .     / s P   4 J      
 �   �                - 
    0  � 1    2   3      K � P   W   O     5   6  V:�U�  7 �L�*�*�  8          9          G s P  H �� J �       )                        �]G- $      
DTRJA061.C                     V  
  [C]EXAMPLE.C;1                                                                                                                 J   �                          ��              #include <stdio> #include <descrip> #include <ssdef>   /*  % Include the DATATRIEVE Access Block.   */   #include <dtr$library:dab.h>  " void report_error (unsigned int );  H #define VERIFY(status)  if ((status) != SS$_NORMAL) report_error(status)   main() {  #define STACK_SIZE 100 #define DTR_BUF_LENGTH 133 #define DTR_CMD_LENGTH 256 #define DTR_ARG_LENGTH 256  9 #define MESSAGE1 "The following domains are available:\n\  \tOWNERS\n\  \tPERSONNEL\n\	 \tYACHTS" C #define MESSAGE2 "\nEnter the name of the domain you want to see: "   #define MESSAGE3 "\nTry again: "   #define READY_CMD "ready !CMD" #define PRINT_CMD "print !CMD"  E DTR_access_block dab;                    /* dab: DTR access block  */    int unsigned   	status, 	previous_status, '         options = DTR$K_SEMI_COLON_OPT, !         stack_size = STACK_SIZE ;    charB         dtr_msg[DTR_BUF_LENGTH],        /* DTR message buffer   */J         dtr_aux[DTR_BUF_LENGTH],        /* DTR auxiliary message buffer */9         dtr_cmd[DTR_CMD_LENGTH],	/* DTR command buffer */ = 	dtr_arg[DTR_ARG_LENGTH],	/* variable for command argument */ 7 	domain[DTR_ARG_LENGTH],		/* variable for user input */  	*return_ptr ;  # $DESCRIPTOR(dtr_msg_desc, dtr_msg); # $DESCRIPTOR(dtr_aux_desc, dtr_aux); # $DESCRIPTOR(dtr_cmd_desc, dtr_cmd); # $DESCRIPTOR(dtr_arg_desc, dtr_arg);    /*  * Initialize the DATATRIEVE Call Interface.  */   status = dtr$init(&dab,                  &stack_size,                 &dtr_msg_desc,                 &dtr_aux_desc,                 &options); VERIFY(status);    /*  = After DATATRIEVE executes this call, it stalls at the command < stallpoint waiting for a command. This situation is the same9 as when interactive DATATRIEVE displays its DTR> prompt.   */ /*  @ Prompt the user to choose a domain and read in the domain name.  */  $ printf("%s%s", MESSAGE1, MESSAGE2 );   do 	{ $ 	if (gets(domain) == NULL) exit(0) ;   	/* ) 	DATATRIEVE is at the command stallpoint. @ 	Use the call dtr$command to pass a READY command to DATATRIEVE.  < 	Use the substitution directive !CMD to pass the domain name 	the user selected.    	*/    	strcpy(dtr_cmd,READY_CMD); - 	dtr_cmd_desc.dsc$w_length = strlen(dtr_cmd);  	strcpy(dtr_arg, domain );- 	dtr_arg_desc.dsc$w_length = strlen(dtr_arg);   = 	status = dtr$command (&dab, &dtr_cmd_desc, &dtr_arg_desc ) ;  	  	/* 4 	Each time DATATRIEVE attempts to execute a command,B 	it puts a condition code in the dab$l_condition field of the dab.  ? 	If DATATRIEVE successfully executes the READY command, it puts / 	the condition DTR$_SUCCESS in dab$l_condition.   ? 	To check for errors, examine the contents of dab$l_condition.    @ 	If the READY command was not completed successfully, DATATRIEVED 	stores an error message in the buffer pointed by the dab$a_msg_buf , 	field and stalls at the message stallpoint.  D 	The call dtr$dtr transfers control to DATATRIEVE. The DTR$M_OPT_CMD4 	value instructs DATATRIEVE to return control to the7 	program when DATATRIEVE is at the command stallpoint.   	*/   ' 	previous_status = dab.dab$l_condition;   ' 	if (previous_status != DTR$_SUCCESS )   		{  		options = DTR$M_OPT_CMD;% 		status = dtr$dtr (&dab, &options) ;  		VERIFY (status) ;                    /*  ) 		DATATRIEVE displays the error message.  3 		It then returns control to the program and waits   		for the next command.  		*/   		/*  - 		Reprompt the user for a valid domain name.   		*/   		printf(MESSAGE3);  		}   , 	} while (previous_status != DTR$_SUCCESS) ;   /*  > DATATRIEVE has successfully executed the READY command and has? a message for the program notifying that the statement has been  successfully completed.   0 DATATRIEVE now stalls at the message stallpoint.  G To continue passing commands to DATATRIEVE, use the call dtr$continue.   */   status = dtr$continue (&dab) ; VERIFY(status);    /*  H DATATRIEVE is now at the command stallpoint, ready for the next command.0 Pass a PRINT domain-name command to DATATRIEVE.  */   strcpy(dtr_cmd,PRINT_CMD);, dtr_cmd_desc.dsc$w_length = strlen(dtr_cmd);  ; status = dtr$command (&dab, &dtr_cmd_desc, &dtr_arg_desc );  VERIFY(status);    /*  < DATATRIEVE now has print lines for the program. It is at the print line stallpoint.  B Use the call dtr$dtr to transfer control to DATATRIEVE. DATATRIEVE= displays the record stream and returns control to the program ' when it is ready for the next command.   */   options = DTR$M_OPT_CMD ; # status = dtr$dtr (&dab, &options) ;  VERIFY(status);    /*   End the Call Interface.  */   status = dtr$finish (&dab);  VERIFY(status);  }  	 ' void report_error (unsigned int status)  { 1 printf("error calling DATATRIEVE: %u\n", status);  exit(0); } ��                                                                                                                                                                                                                                                                                                                                                                          �               * [C]EXAMPLE.COB;1 +  , X   .     / s P   4 C      
 Z                    - 
    0  � 1    2   3      K � P   W   O     5   6 � �u �  7  ��*�*�  8          9          G s P  H �� J �               IDENTIFICATION DIVISION. PROGRAM-ID.   EXAMPLE.C *******************************************************************  ENVIRONMENT DIVISION.  CONFIGURATION SECTION. DATA DIVISION. WORKING-STORAGE SECTION.  C ******************************************************************* C * Copy in the DAB and set up program variables.                   * C *******************************************************************    COPY "DTR$LIBRARY:DAB.LIB".   $ 01 STACK_SIZE PIC 99 COMP VALUE 100.' 01 LINE_NO PIC 9 USAGE IS COMP VALUE 1. ' 01 COL_NO PIC 9  USAGE IS COMP VALUE 1.  01 DOMAIN PIC X(9).  01 COMMAND_LINE PIC X(80).C *******************************************************************  PROCEDURE DIVISION.  010-INITIALIZE-THE-INTERFACE.   ,         CALL "DTR$INIT" USING DAB STACK_SIZE7                         BY DESCRIPTOR MSG_BUFF AUX_BUFF :                         BY REFERENCE DTR$K_SEMI_COLON_OPT.  C ******************************************************************* C * After DATATRIEVE executes this call, it stalls at the           * C * command stallpoint and waits for a command. The situation       * C * is the same as when DATATRIEVE displays its DTR> prompt.        * C *******************************************************************   7 * Use a Run-Time Library procedure to clear the screen.   4         CALL "LIB$ERASE_PAGE" USING LINE_NO, COL_NO.   020-DISPLAY-CHOICES.  7         DISPLAY "The following domains are available:".          DISPLAY "".          DISPLAY " OWNERS".         DISPLAY " PERSONNEL".          DISPLAY " YACHTS".   030-READY-THE-DOMAIN. C ******************************************************************* C * Prompt the user to choose a domain and read in the domain name. * C *******************************************************************            DISPLAY "". A         DISPLAY "Enter the name of the domain you want to see: ".          ACCEPT DOMAIN.  C ******************************************************************* C * DATATRIEVE is at the command stallpoint. Use the call           * C * DTR$COMMAND to pass a READY command to DATATRIEVE.              * C * Use the substitution directive !CMD to pass the domain          * C * name the user selected.                                         * C *******************************************************************   *         MOVE "READY !CMD" TO COMMAND_LINE.$         CALL "DTR$COMMAND" USING DAB=                            BY DESCRIPTOR COMMAND_LINE DOMAIN.   C ******************************************************************* C * The call DTR$DTR transfers control to DATATRIEVE. The           * C * DTR$M_OPT_CMD parameter instructs DATATRIEVE to return          * C * control to the program when DATATRIEVE is at the command        * C * stallpoint. If there is an error in readying the domain,        * C * DATATRIEVE prints the error message and returns control.        * C *******************************************************************   /         CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.   C **********************�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  *                        v+I) $      
DTRJA061.C                     X  
  [C]EXAMPLE.COB;1                                                                                                               C                              s             ********************************************* C * Each time DATATRIEVE attempts to execute a command, it puts     * C * a condition code in the DAB$L_CONDITION field of the DAB.       * C * If DATATRIEVE successfully executes the READY command, it       * C * puts the condition DTR$_SUCCESS in DAB$L_CONDITION.             * C * To check for errors, examine the contents of DAB$L_CONDITION.   * C *******************************************************************   2         IF DAB$L_CONDITION NOT = DTR$_SUCCESS THEN$                 DISPLAY "Try again."+                 GO TO 030-READY-THE-DOMAIN.    040-PRINT-THE-RECORD-STREAM.  C ******************************************************************* C * DATATRIEVE is at the command stallpoint. Pass a PRINT           * C * domain-name statement to DATATRIEVE.                            * C *******************************************************************   *         MOVE "PRINT !CMD" TO COMMAND_LINE.$         CALL "DTR$COMMAND" USING DAB=                            BY DESCRIPTOR COMMAND_LINE DOMAIN.   C ******************************************************************* C * DATATRIEVE now has print lines for the program. It is at the    * C * print line stallpoint. Use the call DTR$DTR to transfer         * C * control to DATATRIEVE. DATATRIEVE displays the record stream    * C * and returns control when it is ready for the next command.      * C *******************************************************************   /         CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.    999-END-THE-INTERFACE.$         CALL "DTR$FINISH" USING DAB.         STOP RUN.   ��                                                                                                                                                                                                                                                                                                                                                                                                                                    �               * [C]EXAMPLE.FOR;1 +  , Z   . 	    / s P   4 H   	    |   �                - 
    0  � 1    2   3      K � P   W   O 	    5   6 �M�ᐕ  7 �W�*�*�  8          9          G s P  H �� J �              & C Include the DATATRIEVE Access Block.  !         INCLUDE 'DTR$LIBRARY:DAB'   + C Declare a variable to receive user input.            CHARACTER*9  DOMAIN  	INTEGER*4   STATUS, LENGTH + 	INTEGER*4   DTR$INIT, DTR$COMMAND, DTR$DTR % 	INTEGER*4   DTR$CONTINUE, DTR$FINISH   + C Initialize the DATATRIEVE Call Interface.   8         STATUS = DTR$INIT (DAB, 100, MSG_BUFF, AUX_BUFF,% 	1              DTR$K_SEMI_COLON_OPT)    	IF (( .NOT. STATUS ) .OR." 	1	( DAB$L_CONDITION .NE. 0)) THEN' 		STOP ' Error initializing DATATRIEVE'  	END IF   ? C After DATATRIEVE executes this call, it stalls at the command > C stallpoint waiting for a command. This situation is the same: C as when interactive DATATRIEVE displays its DTR> prompt.  7 C Use a Run-Time Library procedure to clear the screen.   !         CALL LIB$ERASE_PAGE (1,1)   A C Prompt the user to choose a domain and read in the domain name.    9       WRITE (6, 10) 7 10      FORMAT (' The following domains are available:'  	1       //' OWNERS' 	2        /' PERSONNEL'  	3        /' YACHTS')    20      WRITE (6,30)D 30      FORMAT(/' Enter the name of the domain you want to see: ',$)  0         READ (5, 40, ERR = 9999 ) LENGTH, DOMAIN 40      FORMAT (Q,A)  9 C if no name is entered, we assume the user wants to stop  	IF ( LENGTH .EQ. 0 ) THEN 	   	GOTO 9999 	END IF   $ 	IF ( LENGTH .GT. LEN(DOMAIN) ) THEN" 	   	TYPE *,'Domain name too long' 		GOTO 9 	END IF     * C DATATRIEVE is at the command stallpoint.A C Use the call DTR$COMMAND to pass a READY command to DATATRIEVE.  C = C Use the substitution directive !CMD to pass the domain name  C the user selected.  :         STATUS = DTR$COMMAND (DAB, 'READY !CMD ;', DOMAIN)  5 C Each time DATATRIEVE attempts to execute a command, C C it puts a condition code in the DAB$L_CONDITION field of the DAB.  C @ C If DATATRIEVE successfully executes the READY command, it puts0 C the condition DTR$_SUCCESS in DAB$L_CONDITION. C ? C To check for errors, examine the contents of DAB$L_CONDITION.   9         IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN   A C If the READY command was not completed successfully, DATATRIEVE ? C stores an error message in the DAB$A_MSG_BUF field and stalls  C at the message stallpoint. C E C The call DTR$DTR transfers control to DATATRIEVE. The DTR$M_OPT_CMD 9 C parameter instructs DATATRIEVE to return control to the 7 C program when DATATRIEVE is at the command stallpoint.  C   5                 STATUS = DTR$DTR (DAB, DTR$M_OPT_CMD)   C C DATATRIEVE displays the error message. It then returns control to - C the program and waits for the next command.   , C Reprompt the user for a valid domain name.                   WRITE (6, 50) , 50              FORMAT (/X, 'Try again: ',$)                 GO TO 9          END IF  @ C DATATRIEVE has successfully executed the READY command and has@ C a message for the program: "Statement successfully completed." C 2 C DATATRIEVE now stalls at the message stallpoint. C H C To continue passing commands to DATATRIEVE, use the call DTR$CONTINUE.  #         STATUS = DTR$CONTINUE (DAB)   < C DATATRIEVE is now at the command stallpoint, ready for the C next command.  C 1 C Pass a PRINT domain-name command to DATATRIEVE.   :         STATUS = DTR$COMMAND (DAB, 'PRINT !CMD ;', DOMAIN)  > C DATATRIEVE now has print lines for the program. It is at the C print line stallpoint. C D C Use the call DTR$DTR to transfer control to DATATRIEVE. DATATRIEVE? C displays the record stream and returns control to the program ( C when it is ready for the next command.  -         STATUS = DTR$DTR (DAB, DTR$M_OPT_CMD)    C End the Call Interface.    9999	STATUS = DTR$FINISH (DAB)           END ��                                                                                                                                                                                                                                                                                                                                                                                                  �               * [C]EXAMPLE.PAS;1 +  , ]   .     / s P   4 G       �                   - 
    0  � 1    2   3      K � P   W   O     5   6 �Z��v �  7 @��*�*�  8          9          G s P  H �� J �              % PROGRAM PROGRAM_NAME (INPUT, OUTPUT);   % {Include the DATATRIEVE Access Block}    %INCLUDE 'DTR$LIBRARY:DAB'  * {Declare a procedure to clear the screen.}  G PROCEDURE LIB$ERASE_PAGE (LINE_NO : INTEGER; COL_NO : INTEGER); EXTERN;    PROCEDURE MAIN_ROUTINE;    VAR      LINE_NO : INTEGER;     COL_NO  : INTEGER;"     DOMAIN  : VARYING[9]  OF CHAR;"     COMMAND : VARYING[20] OF CHAR;   BEGIN      LINE_NO := 1;      COL_NO  := 1; 9     LIB$ERASE_PAGE (LINE_NO, COL_NO); {Clear the screen.}   ) {Display the names of available domains.}   5     WRITELN('The following domains are available :');      WRITELN(' ');      WRITELN('OWNERS');     WRITELN('PERSONNEL');      WRITELN('YACHTS');  0     WHILE DAB.DAB$L_CONDITION <> DTR$_SUCCESS DO  ? {When DATATRIEVE successfully executes a command or statement,} 8 {it puts the condition DTR$_SUCCESS in DAB$L_CONDITION.}D {Use the content of DAB$L_CONDITION to control the current DO loop.}  	     BEGIN          WRITELN;  B {Prompt the user for a domain name and read the name into PARAM1.}  G         WRITE       ('Enter the name of the domain you want to see: ');          READLN      (DOMAIN);   * {DATATRIEVE is at the command stallpoint.}A {Use the call DTR$COMMAND to pass a READY command to DATATRIEVE.} G {Use the substitution directive !CMD to pass the selected domain name.}   "         COMMAND :=  'READY !CMD;';+         DTR$COMMAND (DAB, COMMAND, DOMAIN);   A {If the READY command was not completed successfully, DATATRIEVE} < {has an error message and an error code in DAB$L_CONDITION.}- {DATATRIEVE stalls for further instructions.}   E {The call DTR$DTR transfers control to DATATRIEVE. The DTR$M_OPT_CMD} C {argument to this call instructs the terminal server to handle all} C {stallpoints, and return control to the program when DATATRIEVE is}  {ready for the next command.}   )         DTR$DTR     (DAB, DTR$M_OPT_CMD);   ? {DATATRIEVE prints its error message and stalls at the command}  {stallpoint.}        END;  3 {The READY command has been executed successfu�                                                                                                                                                                                                                                                   +                        5�� $      
DTRJA061.C                     ]  
  [C]EXAMPLE.PAS;1                                                                                                               G                              w             lly.} * {DATATRIEVE is at the command stallpoint.}3 {Pass a PRINT domain-name statement to DATATRIEVE.}   !     COMMAND     := 'PRINT !CMD;'; '     DTR$COMMAND (DAB, COMMAND, DOMAIN);   B {DATATRIEVE now has print lines for the program and stalls at the} {print line stallpoint.}  D {Use the call DTR$DTR to transfer control to DATATRIEVE. DATATRIEVE}D {displays the record stream and returns control to the program when}# {it is ready for the next command.}   %     DTR$DTR     (DAB, DTR$M_OPT_CMD);    END;   BEGIN   {Main Program}    {Initialize the Call Interface.}  > DTR$INIT (DAB, 100, MSG_BUFF, AUX_BUFF, DTR$K_SEMI_COLON_OPT);  ? {After DATATRIEVE executes this call, it stalls at the command} A {stallpoint waiting for a command. This situation is the same as} 7 {when interactive DATATRIEVE displays its DTR> prompt.}    MAIN_ROUTINE;   , DTR$FINISH  (DAB); {End the Call Interface.}   END.��                                                                                                                    �               * [C]EXAMPLE.PLI;1 +  , `   . 	    / s P   4 M   	    �                   - 
    0  � 1    2   3      K � P   W   O 	    5   6 @x2w �  7 ��*�*�  8          9          G s P  H �� J �              % DTR_EXAMPLE: PROCEDURE OPTIONS(MAIN);      /*     * Included files and modules.    */ !   %INCLUDE 'DTR$LIBRARY:DAB.PLI';    DCL LIB$ERASE_PAGE ENTRY (       FIXED BINARY(15),        FIXED BINARY(15)) 3       RETURNS (FIXED BINARY(31)) OPTIONS(VARIABLE);      /*    * Local declarations     */ :   DECLARE DOMAIN CHARACTER(32) VARYING;  /* Domain name */@   DECLARE STATUS FIXED BIN(31);   /* For return status values */B   DECLARE 1 DAB LIKE DTR_ACCESS_BLOCK; /* Local copy of the DAB */   DECLARE MSG_BUFF CHAR(80);   DECLARE AUX_BUFF CHAR(20);     /*.    * Initialize the DATATRIEVE Call Interface.    */ 1   STATUS = DTR$INIT(DAB, 100, MSG_BUFF, AUX_BUFF, 4                               DTR$M_SEMI_COLON_OPT);     /*B    * After DATATRIEVE executes this call, it stalls at the commandA    * stallpoint waiting for a command. This situation is the same =    * as when interactive DATATRIEVE displays its DTR> prompt.     *:    * Use a Run-Time Library procedure to clear the screen.    */    STATUS = LIB$ERASE_PAGE(1,1);      /*D    * Prompt the user to choose a domain and read in the domain name.    */ 6   PUT EDIT('The following domains are available:')(A);<   PUT EDIT('OWNERS','PERSONNEL','YACHTS')(SKIP,3(SKIP,X,A));   PUT SKIP(2);     GET LIST(DOMAIN)H       OPTIONS(PROMPT('Enter the name of the domain you want to see: '));     /*-    * DATATRIEVE is at the command stallpoint. D    * Use the call DTR$COMMAND to pass a READY command to DATATRIEVE.    *@    * Use the substitution directive !CMD to pass the domain name    * the user selected.     */ 2   STATUS = DTR$COMMAND(DAB, 'READY !CMD', DOMAIN);     /*8    * Each time DATATRIEVE attempts to execute a command,F    * it puts a condition code in the DAB$L_CONDITION field of the DAB.A    * The status value is also the return value from the function.     *C    * If DATATRIEVE successfully executes the READY command, it puts 3    * the condition DTR$_SUCCESS in DAB$L_CONDITION.     *B    * To check for errors, examine the contents of DAB$L_CONDITION.    */ 0   DO WHILE(DAB.DAB$L_CONDITION ^= DTR$_SUCCESS);  	        /* I         * If the READY command was not completed successfully, DATATRIEVE G         * stores an error message in the DAB$A_MSG_BUF field and stalls $         * at the message stallpoint.	         * M         * The call DTR$DTR transfers control to DATATRIEVE. The DTR$M_OPT_CMD A         * parameter instructs DATATRIEVE to return control to the ?         * program when DATATRIEVE is at the command stallpoint. 
         */,        STATUS = DTR$DTR(DAB, DTR$M_OPT_CMD);  	        /* K         * DATATRIEVE displays the error message. It then returns control to 5         * the program and waits for the next command. 	         * 4         * Reprompt the user for a valid domain-name.
         */7        GET LIST(DOMAIN) OPTIONS(PROMPT('Try again: '));   	        /*          * Retry the operation.
         */7        STATUS = DTR$COMMAND(DAB, 'READY !CMD', DOMAIN);           END;      /*C    * DATATRIEVE has successfully executed the READY command and has C    * a message for the program: "Statement successfully completed."     *5    * DATATRIEVE now stalls at the message stallpoint.     *K    * To continue passing commands to DATATRIEVE, use the call DTR$CONTINUE.     */    STATUS = DTR$CONTINUE(DAB);      /*?    * DATATRIEVE is now at the command stallpoint, ready for the     * next command.    *4    * Pass a PRINT domain-name command to DATATRIEVE.    */ 2   STATUS = DTR$COMMAND(DAB, 'PRINT !CMD', DOMAIN);     /*A    * DATATRIEVE now has print lines for the program. It is at the     * print line stallpoint.     *G    * Use the call DTR$DTR to transfer control to DATATRIEVE. DATATRIEVE B    * displays the record stream and returns control to the program+    * when it is ready for the next command.     */ '   STATUS = DTR$DTR(DAB, DTR$M_OPT_CMD);      /*    * End the Call Interface.    */    STATUS = DTR$FINISH(DAB);      END;��                                                                                                �               * [C]INFO.BAS;1 +  , d   .     / s P   4 )       �                    - 
    0  � 1    2   3      K � P   W   O     5   6 ��z*v�  7 �S�*�*�  8          9 @����  G s P  H �� J �                  	DECLARE INTEGER CONSTANT & ! 	    DTR$K_INF_TYPE_DOMAIN =1%, & % 	    DTR$K_INF_TYPE_COLLECTION =2%, & " 	    DTR$K_INF_TYPE_KEYWORD =3%, &# 	    DTR$K_INF_TYPE_DIC_NAME =4%, &  	    DTR$K_INF_TYPE_GLV =5%, & 	    DTR$K_INF_TYPE_PLOT =6%   	DECLARE INTEGER CONSTANT &  	    DTR$K_INF_DOM_FLD =9%, &  	    DTR$K_INF_DOM_FORM =10%, &   	    DTR$K_INF_DOM_SHARE =11%, &! 	    DTR$K_INF_DOM_ACCESS =12%, &  	    DTR$K_INF_DOM_NAME =13%, & # 	    DTR$K_INF_DOM_NEXT_DOM =14%, &  	    DTR$K_INF_DOM_SSC =15%, & 	    DTR$K_INF_FLD_NAME =16%, &   	    DTR$K_INF_FLD_QNAME =17%, &" 	    DTR$K_INF_FLD_PICTURE =18%, & 	    DTR$K_INF_FLD_EDIT =19%, &   	    DTR$K_INF_FLD_DTYPE =20%, &! 	    DTR$K_INF_FLD_OFFSET =21%, & ! 	    DTR$K_INF_FLD_LENGTH =22%, &   	    DTR$K_INF_FLD_SCALE =23%, &  	    DTR$K_INF_FLD_CHILD =24%, & 	    DTR$K_INF_FLD_CNT =25%, & 	    DTR$K_INF_FLD_LIST =26%, & $ 	    DTR$K_INF_FLD_REDEFINES =27%, &" 	    DTR$K_INF_FLD_VIRTUAL =28%, &! 	    DTR$K_INF_FLD_FILLER =29%, & " 	    DTR$K_INF_FLD_MISSING =30%, &  	    DTR$K_INF_COL_CURSOR =1%, & 	    DTR$K_INF_COL_SIZE =2%, & 	    DTR$K_INF_COL_FLD =3%, & ! 	    DTR$K_INF_COL_DROPPED =4%, &   	    DTR$K_INF_COL_ERASED =5%, &# 	    DTR$K_INF_COL_INVISIBLE =6%, &  	    DTR$K_INF_COL_NAME =7%, &" 	    DTR$K_INF_COL_NEXT_COL =8%, &$ 	    DTR$K_INF_GLV_FIRST_DOM =34%, &$ 	    DTR$K_INF_GLV_FIRST_COL =35%, &$ 	    DTR$K_INF_GLV_FIRST_SSC =36%, & 	    DTR$K_INF_FRM_NAME =32%, & " 	    DTR$K_INF_FRM_LIBRARY =33%, & 	    DTR$K_INF_SSC_NAME =46%, &  	    DTR$K_INF_SSC_SET =47%, & 	    DTR$K_INF_SSC_NEXT =48%, &  	    DTR$K_INF_SET_NAME =49%, &  	    DTR$K_INF_SET_NEXT =50%, &  	    DTR$K_INF_SET_SDP =51%, &# 	    DTR$K_INF_SET_SINGULAR =52%, &  	    DTR$K_INF_SDP_NEXT =53%, & ! 	    DTR$K_INF_SDP_DOMAIN =54%, & " 	    DTR$K_INF_SDP_TENANCY =55%, &! 	    DTR$K_INF_SDP_INSERT =56%, & ! 	    DTR$K_INF_SDP_RETAIN =57%, &  	    DTR$K_INF_FLD_QHDR =31%, &  	    DTR$K_INF_HDR_CNT =40%, &! 	    DTR$K_INF_HDR_STRING =41%, & " 	    DTR$K_INF_GLV_STA_OBJ =37%, &" 	    DTR$K_INF_GLV_STA_CNT =38%, &# 	    DTR$K_INF_GLV_STA_LINE =39%, &  	    DTR$K_INF_PLO_CNT =42%, & 	    DTR$K_INF_PLO_PAI =43%, &! 	    DTR$K_INF_PAI_PROMPT =44%, &   	    DTR$K_INF_PAI_DTYPE =45%, &" 	    DTR$K_INF_DOM_REC_LEN =58%, && 	    DTR$K_INF_FLD_MISSING_TXT =59%, &% 	    DTR$K_INF_FLD_SEG_STRING =60%, &  	    DTR$K_INF_GLV_DEF_DIC =61%    	DECLARE INTEGER CONSTANT & % 	    DTR$K_INF_DOM_ACCESS_READ =1%, & & 	    DTR$K_INF_DOM_ACCESS_WRITE =2%, &' 	    DTR$K_INF_DOM_ACCESS_MODIFY =3%, & ' 	    DTR$K_INF_DOM_ACCESS_EXTEND =4%, & ) 	    DTR$K_INF_DOM_SHARE_EXCLUSIVE =1%, & & 	    DTR$K_INF_DOM_SHARE_SHARED =2%, &' 	    DTR$K_INF_DOM_SHARE_PROTECT =3%, & % 	    DTR$K_INF_DOM_SHARE_SNAPSHOT =4% ��                                                                                                                                                                                                                                                                                                            �               * [C]INFO.COB;1 +  , e   .     / s P   4 J       �                    - 
    0  � 1    2   3      K � P   W   O     5   6  Ʀ�*v�  7  �*�*�  8          9 @����  G s P  H �� J �        ,                        ޙ�w $      
DTRJA061.C                     e  
  [C]INFO.COB;1                                                                                                                  J                              �              01  DTR$INFO_TYPE_CONSTANTS.I     03  DTR$K_INF_TYPE_DOMAIN                  PIC 9(4) COMP VALUE     1. I     03  DTR$K_INF_TYPE_COLLECTION              PIC 9(4) COMP VALUE     2. I     03  DTR$K_INF_TYPE_KEYWORD                 PIC 9(4) COMP VALUE     3. I     03  DTR$K_INF_TYPE_DIC_NAME                PIC 9(4) COMP VALUE     4. I     03  DTR$K_INF_TYPE_GLV                     PIC 9(4) COMP VALUE     5. I     03  DTR$K_INF_TYPE_PLOT                    PIC 9(4) COMP VALUE     6.    01  DTR$LOOKUP_INFO_CONSTANTS.I     03  DTR$K_INF_DOM_FLD                      PIC 9(4) COMP VALUE     9. J     03  DTR$K_INF_DOM_FORM                     PIC 9(4) COMP VALUE     10.J     03  DTR$K_INF_DOM_SHARE                    PIC 9(4) COMP VALUE     11.J     03  DTR$K_INF_DOM_ACCESS                   PIC 9(4) COMP VALUE     12.J     03  DTR$K_INF_DOM_NAME                     PIC 9(4) COMP VALUE     13.J     03  DTR$K_INF_DOM_NEXT_DOM                 PIC 9(4) COMP VALUE     14.J     03  DTR$K_INF_DOM_SSC                      PIC 9(4) COMP VALUE     15.J     03  DTR$K_INF_FLD_NAME                     PIC 9(4) COMP VALUE     16.J     03  DTR$K_INF_FLD_QNAME                    PIC 9(4) COMP VALUE     17.J     03  DTR$K_INF_FLD_PICTURE                  PIC 9(4) COMP VALUE     18.J     03  DTR$K_INF_FLD_EDIT                     PIC 9(4) COMP VALUE     19.J     03  DTR$K_INF_FLD_DTYPE                    PIC 9(4) COMP VALUE     20.J     03  DTR$K_INF_FLD_OFFSET                   PIC 9(4) COMP VALUE     21.J     03  DTR$K_INF_FLD_LENGTH                   PIC 9(4) COMP VALUE     22.J     03  DTR$K_INF_FLD_SCALE                    PIC 9(4) COMP VALUE     23.J     03  DTR$K_INF_FLD_CHILD                    PIC 9(4) COMP VALUE     24.J     03  DTR$K_INF_FLD_CNT                      PIC 9(4) COMP VALUE     25.J     03  DTR$K_INF_FLD_LIST                     PIC 9(4) COMP VALUE     26.J     03  DTR$K_INF_FLD_REDEFINES                PIC 9(4) COMP VALUE     27.J     03  DTR$K_INF_FLD_VIRTUAL                  PIC 9(4) COMP VALUE     28.J     03  DTR$K_INF_FLD_FILLER                   PIC 9(4) COMP VALUE     29.J     03  DTR$K_INF_FLD_MISSING                  PIC 9(4) COMP VALUE     30.I     03  DTR$K_INF_COL_CURSOR                   PIC 9(4) COMP VALUE     1. I     03  DTR$K_INF_COL_SIZE                     PIC 9(4) COMP VALUE     2. I     03  DTR$K_INF_COL_FLD                      PIC 9(4) COMP VALUE     3. I     03  DTR$K_INF_COL_DROPPED                  PIC 9(4) COMP VALUE     4. I     03  DTR$K_INF_COL_ERASED                   PIC 9(4) COMP VALUE     5. I     03  DTR$K_INF_COL_INVISIBLE                PIC 9(4) COMP VALUE     6. I     03  DTR$K_INF_COL_NAME                     PIC 9(4) COMP VALUE     7. I     03  DTR$K_INF_COL_NEXT_COL                 PIC 9(4) COMP VALUE     8. J     03  DTR$K_INF_GLV_FIRST_DOM                PIC 9(4) COMP VALUE     34.J     03  DTR$K_INF_GLV_FIRST_COL                PIC 9(4) COMP VALUE     35.J     03  DTR$K_INF_GLV_FIRST_SSC                PIC 9(4) COMP VALUE     36.J     03  DTR$K_INF_FRM_NAME                     PIC 9(4) COMP VALUE     32.J     03  DTR$K_INF_FRM_LIBRARY                  PIC 9(4) COMP VALUE     33.J     03  DTR$K_INF_SSC_NAME                     PIC 9(4) COMP VALUE     46.J     03  DTR$K_INF_SSC_SET                      PIC 9(4) COMP VALUE     47.J     03  DTR$K_INF_SSC_NEXT                     PIC 9(4) COMP VALUE     48.J     03  DTR$K_INF_SET_NAME                     PIC 9(4) COMP VALUE     49.J     03  DTR$K_INF_SET_NEXT                     PIC 9(4) COMP VALUE     50.J     03  DTR$K_INF_SET_SDP                      PIC 9(4) COMP VALUE     51.J     03  DTR$K_INF_SET_SINGULAR                 PIC 9(4) COMP VALUE     52.J     03  DTR$K_INF_SDP_NEXT                     PIC 9(4) COMP VALUE     53.J     03  DTR$K_INF_SDP_DOMAIN                   PIC 9(4) COMP VALUE     54.J     03  DTR$K_INF_SDP_TENANCY                  PIC 9(4) COMP VALUE     55.J     03  DTR$K_INF_SDP_INSERT                   PIC 9(4) COMP VALUE     56.J     03  DTR$K_INF_SDP_RETAIN                   PIC 9(4) COMP VALUE     57.J     03  DTR$K_INF_FLD_QHDR                     PIC 9(4) COMP VALUE     31.J     03  DTR$K_INF_HDR_CNT                      PIC 9(4) COMP VALUE     40.J     03  DTR$K_INF_HDR_STRING                   PIC 9(4) COMP VALUE     41.J     03  DTR$K_INF_GLV_STA_OBJ                  PIC 9(4) COMP VALUE     37.J     03  DTR$K_INF_GLV_STA_CNT                  PIC 9(4) COMP VALUE     38.J     03  DTR$K_INF_GLV_STA_LINE                 PIC 9(4) COMP VALUE     39.J     03  DTR$K_INF_PLO_CNT                      PIC 9(4) COMP VALUE     42.J     03  DTR$K_INF_PLO_PAI                      PIC 9(4) COMP VALUE     43.J     03  DTR$K_INF_PAI_PROMPT                   PIC 9(4) COMP VALUE     44.J     03  DTR$K_INF_PAI_DTYPE                    PIC 9(4) COMP VALUE     45.J     03  DTR$K_INF_DOM_REC_LEN                  PIC 9(4) COMP VALUE     58.J     03  DTR$K_INF_FLD_MISSING_TXT              PIC 9(4) COMP VALUE     59.J     03  DTR$K_INF_FLD_SEG_STRING               PIC 9(4) COMP VALUE     60.J     03  DTR$K_INF_GLV_DEF_DIC                  PIC 9(4) COMP VALUE     61.    01  DTR$LOOKUP_ANSWER_CONSTANTS.I     03  DTR$K_INF_DOM_ACCESS_READ              PIC 9(4) COMP VALUE     1. I     03  DTR$K_INF_DOM_ACCESS_WRITE             PIC 9(4) COMP VALUE     2. I     03  DTR$K_INF_DOM_ACCESS_MODIFY            PIC 9(4) COMP VALUE     3. I     03  DTR$K_INF_DOM_ACCESS_EXTEND            PIC 9(4) COMP VALUE     4. I     03  DTR$K_INF_DOM_SHARE_EXCLUSIVE          PIC 9(4) COMP VALUE     1. I     03  DTR$K_INF_DOM_SHARE_SHARED             PIC 9(4) COMP VALUE     2. I     03  DTR$K_INF_DOM_SHARE_PROTECT            PIC 9(4) COMP VALUE     3. I     03  DTR$K_INF_DOM_SHARE_SNAPSHOT           PIC 9(4) COMP VALUE     4. ��                                                                                                                                                                                                                                                                                                                                                      �               * [C]INFO.FOR;1 +  , h   . 	    / s P   4 $   	   	 �                   - 
    0  � 1    2   3      K � P   W   O 
    5   6 �jf�*v�  7 @� +�*�  8          9 @����  G s P  H �� J �                  	INTEGER 	1  DTR$K_INF_TYPE_DOMAIN, 	1  DTR$K_INF_TYPE_COLLECTION, 	1  DTR$K_INF_TYPE_KEYWORD,  	1  DTR$K_INF_TYPE_DIC_NAME, 	1  DTR$K_INF_TYPE_GLV,  	1  DTR$K_INF_TYPE_PLOT    	PARAMETER ( 	2  DTR$K_INF_TYPE_DOMAIN=1,  	2  DTR$K_INF_TYPE_COLLECTION=2, 	2  DTR$K_INF_TYPE_KEYWORD=3,  	2  DTR$K_INF_TYPE_DIC_NAME=4, 	2  DTR$K_INF_TYPE_GLV=5,  	2  DTR$K_INF_TYPE_PLOT=6)   	INTEGER 	3  DTR$K_INF_DOM_FLD, 	3  DTR$K_INF_DOM_FORM,  	3  DTR$K_INF_DOM_SHARE, 	3  DTR$K_INF_DOM_ACCESS,  	3  DTR$K_INF_DOM_NAME,  	3  DTR$K_INF_DOM_NEXT_DOM,  	3  DTR$K_INF_DOM_SSC, 	3  DTR$K_INF_FLD_NAME,  	3  DTR$K_INF_FLD_QNAME, 	3  DTR$K_INF_FLD_PICTURE, 	3  DTR$K_INF_FLD_EDIT,  	3  DTR$K_INF_FLD_DTYPE, 	3  DTR$K_INF_FLD_OFFSET,  	3  DTR$K_INF_FLD_LENGTH,  	3  DTR$K_INF_FLD_SCALE, 	3  DTR$K_INF_FLD_CHILD, 	3  DTR$K_INF_FLD_CNT, 	3  DTR$K_INF_FLD_LIST,  	3  DTR$K_INF_FLD_REDEFINES, 	3  DTR$K_INF_FLD_VIRTUAL, 	3  DTR$K_INF_FLD_FILLER,  	3  DTR$K_INF_FLD_MISSING, 	3  DTR$K_INF_COL_CURSOR,  	3  DTR$K_INF_COL_SIZE,  	3  DTR$K_INF_COL_FLD, 	3  DTR$K_INF_COL_DROPPED, 	3  DTR$K_INF_COL_ERASED,  	3  DTR$K_INF_COL_INVISIBLE, 	3  DTR$K_INF_COL_NAME,  	3  DTR$K_INF_COL_NEXT_COL,  	3  DTR$K_INF_GLV_FIRST_DOM, 	3  DTR$K_INF_GLV_FIRST_COL, 	3  DTR$K_INF_GLV_FIRST_SSC, 	3  DTR$K_INF_FRM_NAME,  	3  DTR$K_INF_FRM_LIBRARY, 	3  DTR$K_INF_SSC_NAME,  	3  DTR$K_INF_SSC_SET, 	3  DTR$K_INF_SSC_NEXT,  	3  DTR$K_INF_SET_NAME,  	3  DTR$K_INF_SET_NEXT,  	3  DTR$K_INF_SET_SDP, 	3  DTR$K_INF_SET_SINGULAR,  	3  DTR$K_INF_SDP_NEXT,  	3  DTR$K_INF_SDP_DOMAIN,  	3  DTR$K_INF_SDP_TENANCY    	INTEGER 	3  DTR$K_INF_SDP_INSERT,  	3  DTR$K_INF_SDP_RETAIN,  	3  DTR$K_INF_FLD_QHDR,  	3  DTR$K_INF_HDR_CNT, 	3  DTR$K_INF_HDR_STRING,  	3  DTR$K_INF_GLV_STA_OBJ, 	3  DTR$K_INF_GLV_STA_CNT, 	3  DTR$K_INF_GLV_STA_LINE,  	3  DTR$K_INF_PLO_CNT, 	3  DTR$K_INF_PLO_PAI, 	3  DTR$K_INF_PAI_PROMPT,  	3  DTR$K_INF_PAI_DTYPE, 	3  DTR$K_INF_DOM_REC_LEN, 	3  DTR$K_INF_FLD_MISSING_TXT, 	3  DTR$K_INF_FLD_SEG_STRING,  	3  DTR$K_�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  -                        :n� $      
DTRJA061.C                     h  
  [C]INFO.FOR;1                                                                                                                  $     	                         Z� 
            INF_GLV_DEF_DIC    	PARAMETER ( 	4  DTR$K_INF_DOM_FLD=9, 	4  DTR$K_INF_DOM_FORM=10, 	4  DTR$K_INF_DOM_SHARE=11,  	4  DTR$K_INF_DOM_ACCESS=12, 	4  DTR$K_INF_DOM_NAME=13, 	4  DTR$K_INF_DOM_NEXT_DOM=14, 	4  DTR$K_INF_DOM_SSC=15,  	4  DTR$K_INF_FLD_NAME=16, 	4  DTR$K_INF_FLD_QNAME=17,  	4  DTR$K_INF_FLD_PICTURE=18,  	4  DTR$K_INF_FLD_EDIT=19, 	4  DTR$K_INF_FLD_DTYPE=20,  	4  DTR$K_INF_FLD_OFFSET=21, 	4  DTR$K_INF_FLD_LENGTH=22, 	4  DTR$K_INF_FLD_SCALE=23,  	4  DTR$K_INF_FLD_CHILD=24,  	4  DTR$K_INF_FLD_CNT=25,  	4  DTR$K_INF_FLD_LIST=26, 	4  DTR$K_INF_FLD_REDEFINES=27,  	4  DTR$K_INF_FLD_VIRTUAL=28,  	4  DTR$K_INF_FLD_FILLER=29, 	4  DTR$K_INF_FLD_MISSING=30,  	4  DTR$K_INF_COL_CURSOR=1,  	4  DTR$K_INF_COL_SIZE=2,  	4  DTR$K_INF_COL_FLD=3, 	4  DTR$K_INF_COL_DROPPED=4, 	4  DTR$K_INF_COL_ERASED=5,  	4  DTR$K_INF_COL_INVISIBLE=6, 	4  DTR$K_INF_COL_NAME=7,  	4  DTR$K_INF_COL_NEXT_COL=8,  	4  DTR$K_INF_GLV_FIRST_DOM=34,  	4  DTR$K_INF_GLV_FIRST_COL=35,  	4  DTR$K_INF_GLV_FIRST_SSC=36,  	4  DTR$K_INF_FRM_NAME=32, 	4  DTR$K_INF_FRM_LIBRARY=33,  	4  DTR$K_INF_SSC_NAME=46, 	4  DTR$K_INF_SSC_SET=47,  	4  DTR$K_INF_SSC_NEXT=48, 	4  DTR$K_INF_SET_NAME=49, 	4  DTR$K_INF_SET_NEXT=50, 	4  DTR$K_INF_SET_SDP=51,  	4  DTR$K_INF_SET_SINGULAR=52, 	4  DTR$K_INF_SDP_NEXT=53, 	4  DTR$K_INF_SDP_DOMAIN=54, 	4  DTR$K_INF_SDP_TENANCY=55)    	PARAMETER ( 	4  DTR$K_INF_SDP_INSERT=56, 	4  DTR$K_INF_SDP_RETAIN=57, 	4  DTR$K_INF_FLD_QHDR=31, 	4  DTR$K_INF_HDR_CNT=40,  	4  DTR$K_INF_HDR_STRING=41, 	4  DTR$K_INF_GLV_STA_OBJ=37,  	4  DTR$K_INF_GLV_STA_CNT=38,  	4  DTR$K_INF_GLV_STA_LINE=39, 	4  DTR$K_INF_PLO_CNT=42,  	4  DTR$K_INF_PLO_PAI=43,  	4  DTR$K_INF_PAI_PROMPT=44, 	4  DTR$K_INF_PAI_DTYPE=45,  	4  DTR$K_INF_DOM_REC_LEN=58, ! 	4  DTR$K_INF_FLD_MISSING_TXT=59,   	4  DTR$K_INF_FLD_SEG_STRING=60, 	4  DTR$K_INF_GLV_DEF_DIC=61)    	INTEGER 	5  DTR$K_INF_DOM_ACCESS_READ, 	5  DTR$K_INF_DOM_ACCESS_WRITE,   	5  DTR$K_INF_DOM_ACCESS_MODIFY,  	5  DTR$K_INF_DOM_ACCESS_EXTEND," 	5  DTR$K_INF_DOM_SHARE_EXCLUSIVE, 	5  DTR$K_INF_DOM_SHARE_SHARED,   	5  DTR$K_INF_DOM_SHARE_PROTECT,  	5  DTR$K_INF_DOM_SHARE_SNAPSHOT   	PARAMETER (  	6  DTR$K_INF_DOM_ACCESS_READ=1,! 	6  DTR$K_INF_DOM_ACCESS_WRITE=2, " 	6  DTR$K_INF_DOM_ACCESS_MODIFY=3," 	6  DTR$K_INF_DOM_ACCESS_EXTEND=4,$ 	6  DTR$K_INF_DOM_SHARE_EXCLUSIVE=1,! 	6  DTR$K_INF_DOM_SHARE_SHARED=2, " 	6  DTR$K_INF_DOM_SHARE_PROTECT=3,# 	6  DTR$K_INF_DOM_SHARE_SNAPSHOT=4) ��                                                                                      �               * [C]INFO.H;1 +  , k   .     / s P   4 P       f  �               - 
    0  � 1    2   3      K � P   W   O     5   6 �O8�d�  7 �'+�*�  8          9 @����  G s P  H �� J �                    /**/P /******************************************************************************/P /**                                                                          **/P /**  Copyright (c) 1992                                                      **/P /**  by DIGITAL Equipment Corporation, Maynard, Mass.                        **/P /**                                                                          **/P /**  This software is furnished under a license and may be used and  copied  **/P /**  only  in  accordance  with  the  terms  of  such  license and with the  **/P /**  inclusion of the above copyright notice.  This software or  any  other  **/P /**  copies  thereof may not be provided or otherwise made available to any  **/P /**  other person.  No title to and ownership of  the  software  is  hereby  **/P /**  transferred.                                                            **/P /**                                                                          **/P /**  The information in this software is subject to change  without  notice  **/P /**  and  should  not  be  construed  as  a commitment by DIGITAL Equipment  **/P /**  Corporation.                                                            **/P /**                                                                          **/P /**  DIGITAL assumes no responsibility for the use or  reliability  of  its  **/P /**  software on equipment which is not supplied by DIGITAL.                 **/P /**                                                                          **/P /******************************************************************************/    /*** MODULE info ***/ N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */N /*                                                                          */B /*		   DATATRIEVE INFO AND LOOKUP LITERALS                      */N /*                                                                          */N /*    FUNCTIONAL DESCRIPTION                                                */N /*                                                                          */I /*	These are the LITERALS for the INFO and LOOKUP calls.               */ N /*                                                                          */N /*    VERSION NUMBER                                                        */I /*	6.0                                                                 */ N /*---------------------------------------------------------------------     */N /*                                                                          */N /*##### DTR$INFO_TYPE_CONSTANTS #######################################     */N /*                                                                          */N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Types                                                               */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_TYPE_MIN 1 #define DTR$K_INF_TYPE_DOMAIN 1 # #define DTR$K_INF_TYPE_COLLECTION 2   #define DTR$K_INF_TYPE_KEYWORD 3! #define DTR$K_INF_TYPE_DIC_NAME 4  #define DTR$K_INF_TYPE_GLV 5 #define DTR$K_INF_TYPE_PLOT 6  #define DTR$K_INF_TYPE_MAX 6N /*                                                                          */N /*##### DTR$LOOKUP_INFO_CONSTANTS #####################################     */N /*                                                                          */N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	INFO codes                                                          */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_INFO_MIN 1N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Collections                                                         */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_COL_MIN 1  #define DTR$K_INF_COL_CURSOR 1 #define DTR$K_INF_COL_SIZE 2 #define DTR$K_INF_COL_FLD 3  #define DTR$K_INF_COL_DROPPED 4  #define DTR$K_INF_COL_ERASED 5! #define DTR$K_INF_COL_INVISIBLE 6  #define DTR$K_INF_COL_NAME 7  #define DTR$K_INF_COL_NEXT_COL 8 #define DTR$K_INF_COL_MAX 8 N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Domains                                                             */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_DOM_MIN 9  #define DTR$K_INF_DOM_FLD 9  #define DTR$K_INF_DOM_FORM 10  #define DTR$K_INF_DOM_SHARE 11 #define DTR$K_INF_DOM_ACCESS 12  #define DTR$K_INF_DOM_NAME 13 ! #define DTR$K_INF_DOM_NEXT_DOM 14  #define DTR$K_INF_DOM_SSC 15 #define DTR$K_INF_DOM_MAX 15N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Fields                                                              */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_FLD_MIN 16 #define DTR$K_INF_FLD_NAME 16  #define DTR$K_INF_FLD_QNAME 17  #define DTR$K_INF_FLD_PICTURE 18 #define DTR$K_INF_FLD_EDIT 19  #define DTR$K_INF_FLD_DTYPE 20 #define DTR$K_INF_FLD_OFFSET 21  #define DTR$K_INF_FLD_LENGTH 22  #define DTR$K_INF_FLD_SCALE 23 #define DTR$K_INF_FLD_CHILD 24 #define DTR$K_INF_FLD_CNT 25 #define DTR$K_INF_FLD_LIST 26 " #define DTR$K_INF_FLD_REDEFINES 27  #define DTR$K_INF_FLD_VIRTUAL 28 #define DTR$K_INF_FLD_FILLER 29   #define DTR$K_INF_FLD_MISSING 30 #define DTR$K_�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  .                        U�$ $      
DTRJA061.C                     k  
  [C]INFO.H;1                                                                                                                    P   �                         ��             INF_FLD_QHDR 31  #define DTR$K_INF_FLD_MAX 31N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Forms                                                               */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_FRM_MIN 32 #define DTR$K_INF_FRM_NAME 32   #define DTR$K_INF_FRM_LIBRARY 33 #define DTR$K_INF_FRM_MAX 33N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Object ids of domains, etc.                                         */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_GLV_MIN 34" #define DTR$K_INF_GLV_FIRST_DOM 34" #define DTR$K_INF_GLV_FIRST_COL 35" #define DTR$K_INF_GLV_FIRST_SSC 36 #define DTR$K_INF_GLV_MAX 36N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Statements                                                          */ N /*---------------------------------------------------------------------     */  #define DTR$K_INF_GLV_STA_MIN 37  #define DTR$K_INF_GLV_STA_OBJ 37  #define DTR$K_INF_GLV_STA_CNT 38! #define DTR$K_INF_GLV_STA_LINE 39   #define DTR$K_INF_GLV_STA_MAX 39N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Header lines                                                        */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_HDR_MIN 40 #define DTR$K_INF_HDR_CNT 40 #define DTR$K_INF_HDR_STRING 41  #define DTR$K_INF_HDR_MAX 41N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Plots                                                               */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_PLOTS_MIN 42 #define DTR$K_INF_PLO_CNT 42 #define DTR$K_INF_PLO_PAI 43 #define DTR$K_INF_PAI_PROMPT 44  #define DTR$K_INF_PAI_DTYPE 45 #define DTR$K_INF_PLOTS_MAX 45N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	DBMS                                                                */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_DBMS_MIN 46  #define DTR$K_INF_SSC_NAME 46  #define DTR$K_INF_SSC_SET 47 #define DTR$K_INF_SSC_NEXT 48  #define DTR$K_INF_SET_NAME 49  #define DTR$K_INF_SET_NEXT 50  #define DTR$K_INF_SET_SDP 51! #define DTR$K_INF_SET_SINGULAR 52  #define DTR$K_INF_SDP_NEXT 53  #define DTR$K_INF_SDP_DOMAIN 54   #define DTR$K_INF_SDP_TENANCY 55 #define DTR$K_INF_SDP_INSERT 56  #define DTR$K_INF_SDP_RETAIN 57  #define DTR$K_INF_DBMS_MAX 57 N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Others                                                              */ N /*---------------------------------------------------------------------     */  #define DTR$K_INF_DOM_REC_LEN 58$ #define DTR$K_INF_FLD_MISSING_TXT 59# #define DTR$K_INF_FLD_SEG_STRING 60   #define DTR$K_INF_GLV_DEF_DIC 61N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	End of info codes                                                   */ N /*---------------------------------------------------------------------     */ #define DTR$K_INF_INFO_MAX 61 N /*                                                                          */N /*##### DTR$LOOKUP_ANSWER_CONSTANTS ###################################     */N /*                                                                          */N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Domain Access                                                       */ N /*---------------------------------------------------------------------     */# #define DTR$K_INF_DOM_ACCESS_READ 1 $ #define DTR$K_INF_DOM_ACCESS_WRITE 2% #define DTR$K_INF_DOM_ACCESS_MODIFY 3 % #define DTR$K_INF_DOM_ACCESS_EXTEND 4 N /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++     */I /*	Domain shareability                                                 */ N /*---------------------------------------------------------------------     */' #define DTR$K_INF_DOM_SHARE_EXCLUSIVE 1 $ #define DTR$K_INF_DOM_SHARE_SHARED 2% #define DTR$K_INF_DOM_SHARE_PROTECT 3 & #define DTR$K_INF_DOM_SHARE_SNAPSHOT 4N /*---------------------------------------------------------------------     */��                                                                                                                                                        �               * [C]INFO.MAR;1 +  , m   . 	    / s P   4 K   	    �                   - 
    0  � 1    2   3      K � P   W   O     5   6 ����*v�  7 ��#+�*�  8          9 @����  G s P  H �� J �                 F ;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ; 5 ;                 DATATRIEVE INFO AND LOOKUP LITERALS  ; K ;       Copyright (c) Digital Equipment Corporation 1981, 1982, 1983, 1986. . ;                         All rights reserved. ; H ; This software is furnished under a license and may be used and  copiedH ; only  in  accordance  with  the  terms  of  such  license and with theH ; inclusion of the above copyright notice.  This software or  any  otherH ; copies  thereof may not be provided or otherwise made available to anyH ; other person.  No title to and ownership of  the  software  is  hereby ; transferred. ; H ; The information in this software is subject to change  without  noticeH ; and  should  not  be  construed  as  a commitment by Digital Equipment ; Corporation. ; H ; Digital assumes no responsibility for the use or  reliability  of  its9 ; software on equipment which is not supplied by Digital.  ;  ;    FUNCTIONAL DESCRIPTION  ; F ;      These are the GLOBAL ASSIGNMENTS for the INFO and LOOKUP calls. ;  ;    VERSION NUMBER 
 ;      1.0F ;--------------------------------------------------------------------- ;<FF>    DTR$K_INF_TYPE_MIN==1  DTR$K_INF_TYPE_DOMAIN==1 DTR$K_INF_TYPE_COLLECTION==2 DTR$K_INF_TYPE_KEYWORD==3  DTR$K_INF_TYPE_DIC_NAME==4 DTR$K_INF_TYPE_GLV==5  DTR$K_INF_TYPE_PLOT==6 DTR$K_INF_TYPE_MAX==6    DTR$K_INF_INFO_MIN==1  DTR$K_INF_DOM_MIN==9 DTR$K_INF_DOM_FLD==9 DTR$K_INF_DOM_FORM==10 DTR$K_INF_DOM_SHARE==11  DTR$K_INF_DOM_ACCESS==12 DTR$K_INF_DOM_NAME==13 DTR$K_INF_DOM_NEXT_DOM==14 DTR$K_INF_DOM_SSC==15  DTR$K_INF_DOM_MAX==15  DTR$K_INF_FLD_MIN==16  DTR$K_INF_FLD_NAME==16 DTR$K_INF_FLD_QNAME==17  DTR$K_INF_FLD_PICTURE==18  DTR$K_INF_FLD_EDIT==19 DTR$K_INF_FLD_DTYPE==20  DTR$K_INF_FLD_OFFSET==21 DTR$K_INF_FLD_LENGTH==22 DTR$K_INF_FLD_SCALE==23  DTR$K_INF_FLD_CHILD==24  DTR$K_INF_FLD_CNT==25  DTR$K_INF_FLD_LIST==26 DTR$K_INF_FLD_REDEFINES==27  DTR$K_INF_FLD_VIRTUAL==28  DTR$K_INF_FLD_FILLER==29 DTR$K_INF_FLD_MISSING==30  DTR$K_INF_FLD_MAX==31  DTR$K_INF_COL_MIN==1 DTR$K_INF_COL_CURSOR==1  DTR$K_INF_COL_SIZE==2  DTR$K_INF_COL_FLD==3 DTR$K_INF_COL_DROPPED==4 DTR$K_INF_COL_ERASED==5  DTR$K_INF_COL_INVISIBLE==6 DTR$K_INF_COL_NAME==7  DTR$K_INF_COL_NEXT_COL==8  DTR$K_INF_COL_MAX==8 DTR$K_INF_GLV_MIN==34  DTR$K_INF_GLV_FIRST_DOM==34  DTR$K_INF_GLV_FIRST_COL==35  DTR$K_INF_GLV_FIRST_SSC==36  DTR$K_INF_GLV_MAX==37  DTR$K_INF_FRM_NAME==32 DTR$K_INF_FRM_LIBRARY==33  DTR$K_INF_SSC_NAME==46 DTR$K_INF_SSC_SET==47  DTR$K_INF_SSC_NEXT==48 DTR$K_INF_SET_NAME==49 DTR$K_INF_SET_NEXT==50 DTR$K_INF_SET_SDP==51  DTR$K_INF_SET_SINGULAR==52 DTR$K_INF_SDP_NEXT==53 DTR$K_INF_SDP_DOMAIN==54 DTR$K_INF_SDP_TENANCY==55  DTR$K_INF_SDP_INSERT==56 DTR$K_INF_SDP_RETAIN==57 DTR$K_INF_FLD_QHDR==31 DTR$K_INF_HDR_CNT==40  DTR$K_INF_HDR_STRING==41 DTR$K_INF_GLV_STA_OBJ==37  DTR$K_INF_GLV_STA_CNT==38  DTR$K_INF_GLV_STA_LINE==39 DTR$K_INF_PLO_CNT==42  DTR$K_INF_PLO_PAI==43  DTR$K_INF_PAI_PROMPT==44 DTR$K_INF_PAI_DTYPE==45  DTR$K_INF_DOM_REC_LEN==58  DTR$K_INF_FLD_MISSING_TXT==59  DTR$K_INF_FLD_SEG_STRING==60 DTR$K_INF_GLV_DEF_DIC==61  DTR$K_INF_INFO_MAX==61   DTR$K_INF_DOM_ACCESS_READ==1 DTR$K_INF_DOM_ACCESS_WRITE==2  DTR$K_INF_DOM_ACCESS_MODIFY==3 DTR$K_INF_DOM_ACCESS_EXTEND==4  DTR$K_INF_DOM_SHARE_EXCLUSIVE==1 DTR$K_INF_DOM_SHARE_SHARED==2  DTR$K_INF_DOM_SHARE_PROTECT==3 DTR$K_INF_DOM_SHARE_SNAPSHOT==4 ��                                                                                                    �               * [C]INFO.PAS;1 +  , o   .     / s P   4 $       b                   - 
    0  � 1    2   3      K � P   W   O     5   6  J~�*v�  7 ��7+�*�  8          9 @����  G s P  H �� J �    �                                                                                                                                                                                                                                                                   /                        � $      
DTRJA061.C                     o  
  [C]INFO.PAS;1                                                                                                                  $                              X� 
             {DTR$INFO TYPE CONSTANTS}      DTR$K_INF_TYPE_DOMAIN=1;      DTR$K_INF_TYPE_COLLECTION=2;     DTR$K_INF_TYPE_KEYWORD=3;      DTR$K_INF_TYPE_DIC_NAME=4;     DTR$K_INF_TYPE_GLV=5;      DTR$K_INF_TYPE_PLOT=6;   {DTR$LOOKUP INFO CONSTANTS}      DTR$K_INF_DOM_FLD=9;     DTR$K_INF_DOM_FORM=10;     DTR$K_INF_DOM_SHARE=11;      DTR$K_INF_DOM_ACCESS=12;     DTR$K_INF_DOM_NAME=13;     DTR$K_INF_DOM_NEXT_DOM=14;     DTR$K_INF_DOM_SSC=15;      DTR$K_INF_FLD_NAME=16;     DTR$K_INF_FLD_QNAME=17;      DTR$K_INF_FLD_PICTURE=18;      DTR$K_INF_FLD_EDIT=19;     DTR$K_INF_FLD_DTYPE=20;      DTR$K_INF_FLD_OFFSET=21;     DTR$K_INF_FLD_LENGTH=22;     DTR$K_INF_FLD_SCALE=23;      DTR$K_INF_FLD_CHILD=24;      DTR$K_INF_FLD_CNT=25;      DTR$K_INF_FLD_LIST=26;     DTR$K_INF_FLD_REDEFINES=27;      DTR$K_INF_FLD_VIRTUAL=28;      DTR$K_INF_FLD_FILLER=29;     DTR$K_INF_FLD_MISSING=30;      DTR$K_INF_COL_CURSOR=1;      DTR$K_INF_COL_SIZE=2;      DTR$K_INF_COL_FLD=3;     DTR$K_INF_COL_DROPPED=4;     DTR$K_INF_COL_ERASED=5;      DTR$K_INF_COL_INVISIBLE=6;     DTR$K_INF_COL_NAME=7;      DTR$K_INF_COL_NEXT_COL=8;      DTR$K_INF_GLV_FIRST_DOM=34;      DTR$K_INF_GLV_FIRST_COL=35;      DTR$K_INF_GLV_FIRST_SSC=36;      DTR$K_INF_FRM_NAME=32;     DTR$K_INF_FRM_LIBRARY=33;      DTR$K_INF_SSC_NAME=46;     DTR$K_INF_SSC_SET=47;      DTR$K_INF_SSC_NEXT=48;     DTR$K_INF_SET_NAME=49;     DTR$K_INF_SET_NEXT=50;     DTR$K_INF_SET_SDP=51;      DTR$K_INF_SET_SINGULAR=52;     DTR$K_INF_SDP_NEXT=53;     DTR$K_INF_SDP_DOMAIN=54;     DTR$K_INF_SDP_TENANCY=55;      DTR$K_INF_SDP_INSERT=56;     DTR$K_INF_SDP_RETAIN=57;     DTR$K_INF_FLD_QHDR=31;     DTR$K_INF_HDR_CNT=40;      DTR$K_INF_HDR_STRING=41;     DTR$K_INF_GLV_STA_OBJ=37;      DTR$K_INF_GLV_STA_CNT=38;      DTR$K_INF_GLV_STA_LINE=39;     DTR$K_INF_PLO_CNT=42;      DTR$K_INF_PLO_PAI=43;      DTR$K_INF_PAI_PROMPT=44;     DTR$K_INF_PAI_DTYPE=45;      DTR$K_INF_DOM_REC_LEN=58; !     DTR$K_INF_FLD_MISSING_TXT=59;       DTR$K_INF_FLD_SEG_STRING=60;     DTR$K_INF_GLV_DEF_DIC=61;    {DTR$LOOKUP ANSWER CONSTANTS}       DTR$K_INF_DOM_ACCESS_READ=1;!     DTR$K_INF_DOM_ACCESS_WRITE=2; "     DTR$K_INF_DOM_ACCESS_MODIFY=3;"     DTR$K_INF_DOM_ACCESS_EXTEND=4;$     DTR$K_INF_DOM_SHARE_EXCLUSIVE=1;!     DTR$K_INF_DOM_SHARE_SHARED=2; "     DTR$K_INF_DOM_SHARE_PROTECT=3;#     DTR$K_INF_DOM_SHARE_SNAPSHOT=4; ��                                                                                                                                                            �               * [C]INFO.PLI;1 +  , q   . 	    / s P   4 0   	    �                    - 
    0  � 1    2   3      K � P   W   O     5   6  X��*v�  7 @#J+�*�  8          9 @����  G s P  H �� J �                  /* DTR$INFO TYPE CONSTANTS */   (     %replace DTR$K_INF_TYPE_DOMAIN by 1;,     %replace DTR$K_INF_TYPE_COLLECTION by 2;)     %replace DTR$K_INF_TYPE_KEYWORD by 3; *     %replace DTR$K_INF_TYPE_DIC_NAME by 4;%     %replace DTR$K_INF_TYPE_GLV by 5; &     %replace DTR$K_INF_TYPE_PLOT by 6;   /* DTR$LOOKUP INFO CONSTANTS */   $     %replace DTR$K_INF_DOM_FLD by 9;&     %replace DTR$K_INF_DOM_FORM by 10;'     %replace DTR$K_INF_DOM_SHARE by 11; (     %replace DTR$K_INF_DOM_ACCESS by 12;&     %replace DTR$K_INF_DOM_NAME by 13;*     %replace DTR$K_INF_DOM_NEXT_DOM by 14;%     %replace DTR$K_INF_DOM_SSC by 15; &     %replace DTR$K_INF_FLD_NAME by 16;'     %replace DTR$K_INF_FLD_QNAME by 17; )     %replace DTR$K_INF_FLD_PICTURE by 18; &     %replace DTR$K_INF_FLD_EDIT by 19;'     %replace DTR$K_INF_FLD_DTYPE by 20; (     %replace DTR$K_INF_FLD_OFFSET by 21;(     %replace DTR$K_INF_FLD_LENGTH by 22;'     %replace DTR$K_INF_FLD_SCALE by 23; '     %replace DTR$K_INF_FLD_CHILD by 24; %     %replace DTR$K_INF_FLD_CNT by 25; &     %replace DTR$K_INF_FLD_LIST by 26;+     %replace DTR$K_INF_FLD_REDEFINES by 27; )     %replace DTR$K_INF_FLD_VIRTUAL by 28; (     %replace DTR$K_INF_FLD_FILLER by 29;)     %replace DTR$K_INF_FLD_MISSING by 30; '     %replace DTR$K_INF_COL_CURSOR by 1; %     %replace DTR$K_INF_COL_SIZE by 2; $     %replace DTR$K_INF_COL_FLD by 3;(     %replace DTR$K_INF_COL_DROPPED by 4;'     %replace DTR$K_INF_COL_ERASED by 5; *     %replace DTR$K_INF_COL_INVISIBLE by 6;%     %replace DTR$K_INF_COL_NAME by 7; )     %replace DTR$K_INF_COL_NEXT_COL by 8; +     %replace DTR$K_INF_GLV_FIRST_DOM by 34; +     %replace DTR$K_INF_GLV_FIRST_COL by 35; +     %replace DTR$K_INF_GLV_FIRST_SSC by 36; &     %replace DTR$K_INF_FRM_NAME by 32;)     %replace DTR$K_INF_FRM_LIBRARY by 33; &     %replace DTR$K_INF_SSC_NAME by 46;%     %replace DTR$K_INF_SSC_SET by 47; &     %replace DTR$K_INF_SSC_NEXT by 48;&     %replace DTR$K_INF_SET_NAME by 49;&     %replace DTR$K_INF_SET_NEXT by 50;%     %replace DTR$K_INF_SET_SDP by 51; *     %replace DTR$K_INF_SET_SINGULAR by 52;&     %replace DTR$K_INF_SDP_NEXT by 53;(     %replace DTR$K_INF_SDP_DOMAIN by 54;)     %replace DTR$K_INF_SDP_TENANCY by 55; (     %replace DTR$K_INF_SDP_INSERT by 56;(     %replace DTR$K_INF_SDP_RETAIN by 57;&     %replace DTR$K_INF_FLD_QHDR by 31;%     %replace DTR$K_INF_HDR_CNT by 40; (     %replace DTR$K_INF_HDR_STRING by 41;)     %replace DTR$K_INF_GLV_STA_OBJ by 37; )     %replace DTR$K_INF_GLV_STA_CNT by 38; *     %replace DTR$K_INF_GLV_STA_LINE by 39;%     %replace DTR$K_INF_PLO_CNT by 42; %     %replace DTR$K_INF_PLO_PAI by 43; (     %replace DTR$K_INF_PAI_PROMPT by 44;'     %replace DTR$K_INF_PAI_DTYPE by 45; )     %replace DTR$K_INF_DOM_REC_LEN by 58; -     %replace DTR$K_INF_FLD_MISSING_TXT by 59; ,     %replace DTR$K_INF_FLD_SEG_STRING by 60;)     %replace DTR$K_INF_GLV_DEF_DIC by 61;   ! /* DTR$LOOKUP ANSWER CONSTANTS */   ,     %replace DTR$K_INF_DOM_ACCESS_READ by 1;-     %replace DTR$K_INF_DOM_ACCESS_WRITE by 2; .     %replace DTR$K_INF_DOM_ACCESS_MODIFY by 3;.     %replace DTR$K_INF_DOM_ACCESS_EXTEND by 4;0     %replace DTR$K_INF_DOM_SHARE_EXCLUSIVE by 1;-     %replace DTR$K_INF_DOM_SHARE_SHARED by 2; .     %replace DTR$K_INF_DOM_SHARE_PROTECT by 3;/     %replace DTR$K_INF_DOM_SHARE_SNAPSHOT by 4; ��                                                                                                                                                                                                                                                                      �               * [C]INFO.REQ;1 +  , r   . 	    / s P   4 H   	    �                   - 
    0  � 1    2   3      K � P   W   O     5   6  ���\�  7 �r\+�*�  8          9 @����  G s P  H �� J �                 F !+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ! ) !		   DATATRIEVE INFO AND LOOKUP LITERALS  ! E !       Copyright (c) Digital Equipment Corporation 1981, 1982, 1983.  !			   All rights reserved.  ! H ! This software is furnished under a license and may be used and  copiedH ! only  in  accordance  with  the  terms  of  such  license and with theH ! inclusion of the above copyright notice.  This software or  any  otherH ! copies  thereof may not be provided or otherwise made available to anyH ! other person.  No title to and ownership of  the  software  is  hereby ! transferred. ! H ! The information in this software is subject to change  without  noticeH ! and  should  not  be  construed  as  a commitment by Digital Equipment ! Corporation. ! H ! Digital assumes no responsibility for the use or  reliability  of  its9 ! software on equipment which is not supplied by Digital.  !  !    FUNCTIONAL DESCRIPTION  ! 7 !	These are the LITERALS for the INFO and LOOKUP calls.  !  !    VERSION NUMBER  !	1.0 H !---------------------------------------------------------------------  LITERAL  DTR$K_INF_TYPE_MIN = 1,  DTR$K_INF_TYPE_DOMAIN =1,  DTR$K_INF_TYPE_COLLECTION =2,  DTR$K_INF_TYPE_KEYWORD =3, DTR$K_INF_TYPE_DIC_NAME =4,  DTR$K_INF_TYPE_GLV =5, DTR$K_INF_TYPE_PLOT =6,  DTR$K_INF_TYPE_MAX =6,   DTR$K_INF_DOM_ACCESS_READ =1,  DTR$K_INF_DOM_ACCESS_WRITE =2, DTR$K_INF_DOM_ACCESS_MODIFY =3,  DTR$K_INF_DOM_ACCESS_EXTEND =4, ! DTR$K_INF_DOM_SHARE_EXCLUSIVE =1,  DTR$K_INF_DOM_SHARE_SHARED =2, DTR$K_INF_DOM_SHARE_PROTECT =3,   DTR$K_INF_DOM_SHARE_SNAPSHOT =4, DTR$K_INF_INFO_MIN = 1,  DTR$K_INF_DOM_MIN =9,  DTR$K_INF_DOM_FLD =9,  DTR$K_INF_DOM_FORM =10,  DTR$K_INF_DOM_SHARE =11, DTR$K_INF_DOM_ACCESS =12,  DTR$K_INF_DOM_NAME =13,  DTR$K_INF_DOM_NEXT_DOM =14,  DTR$K_INF_DOM_SSC =15, DTR$K_INF_DOM_MAX =15, DTR$K_INF_FLD_MIN =16, DTR$K_INF_FLD_NAME =16,  DTR$K_INF_FLD_QNAME =17, DTR$K_INF_FLD_PICTURE =18, DTR$K_INF_FLD_EDIT =19,  DTR$K_INF_FLD_DTYPE =20, DTR$K_IN�                                                                                                                                                                                                                                                   0                        pP� $      
DTRJA061.C                     r  
  [C]INFO.REQ;1                                                                                                                  H     	                         �z             F_FLD_OFFSET =21,  DTR$K_INF_FLD_LENGTH =22,  DTR$K_INF_FLD_SCALE =23, DTR$K_INF_FLD_CHILD =24, DTR$K_INF_FLD_CNT =25, DTR$K_INF_FLD_LIST =26,  DTR$K_INF_FLD_REDEFINES =27, DTR$K_INF_FLD_VIRTUAL =28, DTR$K_INF_FLD_FILLER =29,  DTR$K_INF_FLD_MISSING =30, DTR$K_INF_FLD_MAX =31, DTR$K_INF_COL_MIN =1,  DTR$K_INF_COL_CURSOR =1, DTR$K_INF_COL_SIZE =2, DTR$K_INF_COL_FLD =3,  DTR$K_INF_COL_DROPPED =4,  DTR$K_INF_COL_ERASED =5, DTR$K_INF_COL_INVISIBLE =6,  DTR$K_INF_COL_NAME =7, DTR$K_INF_COL_NEXT_COL =8, DTR$K_INF_COL_MAX =8,  DTR$K_INF_GLV_MIN =34, DTR$K_INF_GLV_FIRST_DOM =34, DTR$K_INF_GLV_FIRST_COL =35, DTR$K_INF_GLV_FIRST_SSC =36, DTR$K_INF_GLV_MAX =37, DTR$K_INF_FRM_NAME =32,  DTR$K_INF_FRM_LIBRARY =33, DTR$K_INF_SSC_NAME =46,  DTR$K_INF_SSC_SET =47, DTR$K_INF_SSC_NEXT =48,  DTR$K_INF_SET_NAME =49,  DTR$K_INF_SET_NEXT =50,  DTR$K_INF_SET_SDP =51, DTR$K_INF_SET_SINGULAR =52,  DTR$K_INF_SDP_NEXT =53,  DTR$K_INF_SDP_DOMAIN =54,  DTR$K_INF_SDP_TENANCY =55, DTR$K_INF_SDP_INSERT =56,  DTR$K_INF_SDP_RETAIN =57,  DTR$K_INF_FLD_QHDR =31,  DTR$K_INF_HDR_CNT =40, DTR$K_INF_HDR_STRING =41,  DTR$K_INF_GLV_STA_OBJ =37, DTR$K_INF_GLV_STA_CNT =38, DTR$K_INF_GLV_STA_LINE =39,  DTR$K_INF_PLO_CNT =42, DTR$K_INF_PLO_PAI =43, DTR$K_INF_PAI_PROMPT =44,  DTR$K_INF_PAI_DTYPE =45, DTR$K_INF_DOM_REC_LEN =58, DTR$K_INF_FLD_MISSING_TXT =59, DTR$K_INF_FLD_SEG_STRING =60,  DTR$K_INF_GLV_DEF_DIC =61, DTR$K_INF_INFO_MAX =61; ��                                                                  �               * [C]LINEAR.BAS;1 +  , w   .     / s P   4 P       �  �                - 
    0  � 1    2   3      K � P   W   O     5   6 @�*,)�  7 ��h+�*�  8          9          G s P  H �� J �                1	%TITLE "LINEAR" N 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!K 	! To run this program, you must compile it and link it with the DATATRIEVE  	! shareable image:  	! 	!  $ BASIC LINEAR 	!  $ LINK LINEAR, DTR/OPT 	!  $ RUN LINEAR 	!G 	! See the VAX DATATRIEVE Guide to Programming and Customizing for more M 	! information on using an option file to link programs that call DATATRIEVE. O 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  	!G 	! The program LINEAR performs  a  linear  regression  on  data  from a C 	! DATATRIEVE domain. You can use this program to check whether two P 	! fields have a linear relationship, that is, whether there are numbers A and B% 	! such that FIELD1 = A + B * FIELD2.  	!O 	! The program first prompts the user for the names of a domain and two fields. P 	! The program  next prompts for a DATATRIEVE FIND statement. The FIND statementI 	! determines which records are used in the regression.  The program then O 	! determines the regression coefficients and prints them. Finally, the program K 	! enables the user to see how close the relationship is to being linear by 5 	! displaying the actual and estimated  field values. P 	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  50	ON ERROR GO TO 8000  ' 	! Include the DATATRIEVE Access Block.   " 100	%INCLUDE "DTR$LIBRARY:DAB.BAS"  1 	! Declare the initialization call as a function. " 	! Declare the normal status code.  # 	EXTERNAL INTEGER FUNCTION DTR$INIT # 	EXTERNAL LONG CONSTANT SS$_NORMAL	    	DECLARE STRING ANSWER   	DECLARE INTEGER RET_STATUS   0 	! Format the record buffer and declare a table.  $ 	MAP (VALUEAREA) REAL VALUE1, VALUE2" 	MAP (VALUEAREA) STRING VALUES = 8 	DIMENSION AVERAGES(2)  5 	! Declare the DTR$DTR options and assign the values.    	DECLARE INTEGER DTR_OPTIONS. 	DTR_OPTIONS = DTR$M_OPT_CMD + DTR$M_OPT_PGET   * 	! Initialize the session with DATATRIEVE.  ? 500	RET_STATUS = DTR$INIT (DAB BY REF, 100% BY REF, MSG_BUFF, & ( 		AUX_BUFF, DTR$K_SEMI_COLON_OPT BY REF)  , 	! Verify that the initialization succeeded.  " 	IF RET_STATUS <> SS$_NORMAL THEN 0 	     PRINT "DATATRIEVE initialization failed."  	     GOTO 8300   / 	! Declare the ports to be used in the program.   # 750	CALL DTR$COMMAND (DAB BY REF, & , 			 "DECLARE PORT PT1 01 N PIC 9(5) COMP.;")  ) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)     	CALL DTR$COMMAND (DAB BY REF, &" 			  "DECLARE PORT PT2 01 WHOLE.")  	CALL DTR$COMMAND (DAB BY REF, &( 			  "02 PART-A REAL. 02 PART-B REAL.;")  : 	! Use DTR$DTR to print any messages and return control to  	! the program on DTR$K_STL_CMD.  ) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   ' 	! Show the user the available domains.   3 1000 	CALL DTR$COMMAND (DAB BY REF, "SHOW DOMAINS") * 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD )  : 	! Prompt the user for the domain to be used and ready it.  0 	INPUT "What is the name of the domain"; DOMAIN$4 	CALL DTR$COMMAND (DAB BY REF, "READY !CMD",DOMAIN$)) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   # 	! Check if the domain was readied.   # 	IF DAB$L_CONDITION <> DTR$_SUCCESS  	   THEN GOTO 1000 	        	        ; 	! Prompt the user to enter a command to form a collection. ) 	! Pass the command string to DATATRIEVE.   1 2000	PRINT "Enter a command to form a collection"  	INPUT COMMAND$ ( 	CALL DTR$COMMAND (DAB BY REF, COMMAND$)) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD) # 	IF DAB$L_CONDITION <> DTR$_SUCCESS  	   THEN GOTO 2000  6 	! Store the count of the number of records found into? 	! a port then pass it to the program in the variable COUNTER%.   ? 2750	CALL DTR$COMMAND (DAB BY REF, "STORE PT1 USING N = COUNT") ) 	CALL DTR$GET_PORT BY REF (DAB, COUNTER%) ' 	CALL DTR$DTR BY REF (DAB, DTR_OPTIONS)   > 	! If there are no records, return to the section that prompts 	! for a collection.    	IF COUNTER% = 0% THEN GOTO 2000   2800	REM							      &G !*********************************************************************& 5 !	Formulas used to find the linear equation:		      &  !								      &* !	LINEAR EQUATION :  Y = bX + a				      & !								      &1 !	Equation to arrive at value for b:   			      & # !			(note:  E = summation			      & , !				n = number of data elements used)     & !								      &3 !		b = E(X*Y) - n(average(X) * average(Y))		      & 3 !		    -----------------------------------		      & + !		    E(X**2) - n(average(x)**2)			      &  !								      &1 !	Equation to arrive at value for a:   			      &  !								      &- !		a = average(Y) - (b * average(X))		      &  !								      &G !**********************************************************************   & 	! Show the user the available fields.  B 3000	CALL DTR$COMMAND (DAB BY REF, "SHOW FIELDS FOR !CMD",DOMAIN$)) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   4 	! Prompt the user for the names of the independent  	! and dependent fields.  ; 	INPUT "What is the name of the independent field"; FIELD1$ 9 	INPUT "What is the name of the dependent field"; FIELD2$   ; 	! Store the total added value of the fields chosen into a   	! port.  8  	CALL DTR$COMMAND (DAB BY REF, "STORE PT2 USING BEGIN")8 	CALL DTR$COMMAND (DAB BY REF, "PART-A = TOTAL !CMD;", & 			  FIELD1$) = 	CALL DTR$COMMAND (DAB BY REF, "PART-B = TOTAL !CMD; END;", &  			  FIELD2$) ' 	CALL DTR$DTR BY REF (DAB, DTR_OPTIONS)   ; 	! Use DTR$GET_PORT to retrieve totals from DATATRIEVE and  % 	! store them in the variable VALUES.   , 3500	CALL DTR$GET_PORT BY REF (DAB , VALUES)) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD) # 	IF DAB$L_CONDITION <> DTR$_SUCCESS  	   THEN GOTO 3000  8 	! Find the average value of each field (total / number  	! of records).   % 3750	AVERAGES(1) = VALUE1 / COUNTER%    	AVERAGES(2) = VALUE2 / COUNTER%  8 	! One at a time, store the value of the fields in each  	! record into port PT2.    	CALL DTR$COMMAND (DAB BY REF, &) 			  "FOR CURRENT STORE PT2 USING BEGIN")   	CALL DTR$COMMAND (DAB BY REF, &, 			  "PART-A = !CMD; PART-B = !CMD; END;", & 			   FIELD1$, FIELD2$)   5 	! The DATATRIEVE statement storing a port caused the  	! stallpoint DTR$K_STL_PGET.  	!> 	! Use DTR$GET_PORT to retrieve the values stored in the port.  , 4000	CALL DTR$GET_PORT BY REF (DAB , VALUES)  = 	! Calculate the summations needed for the linear regression.   , 	SUMXY = SUMXY + (VALUE1 * VALUE2)		! E(XY).) 	SUMX2 = SUMX2 + (VALUE1**2)			! E(X**2).   A 	! Continue retrieving values until the whole collection has been 
 	! stored.  / 	IF DAB$W_STATE = DTR$K_STL_PGET THEN GOTO 4000   - 4500	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)   9 	! Calculate the numerator and d�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  1                        Y�g $      
DTRJA061.C                     w  
  [C]LINEAR.BAS;1                                                                                                                P   �                          /             enominator of the linear   	! regression equation.   8 	TOP = (SUMXY - (COUNTER% * AVERAGES(1) * AVERAGES(2))) / 	BOTTOM = (SUMX2 - (COUNTER% * AVERAGES(1)**2))   = 	! Calculate the values of the slope (b) and y-intercept (a).    	b = TOP/BOTTOM $ 	a = AVERAGES(2) - (b * AVERAGES(1))  ! 	! Print out the linear equation.   0 	PRINT "Best estimate for linear relation is..."3 	PRINT FIELD2$; " = "; a; " + "; b ; " * "; FIELD1$   ? 	! Ask if the user wishes to see the calculated values and the   	! actual values.   8 	INPUT "Enter Y if you want to see relationship"; ANSWER  	IF ANSWER = "Y" THEN GOSUB 6000  . 	! Prompt for what the user wishes to do next.  * 5000	PRINT "Enter CTRL/Z to exit program."/ 	PRINT "Enter 1 to start over with new domain." 3 	PRINT "Enter 2 to start over with new collection." : 	PRINT "Enter 3 to use same collection, different fields."  	 	INPUT D%   6 	! If the user did not enter 1, 2, or 3, prompt again.   5250	IF (D% > 3%) OR (D% < 1%)+ 	   THEN PRINT "Invalid entry, try again."   		GOTO 5000    	! Clear all variables.    5500	VALUE1 = 0.0  	VALUE2 = 0.0  	AVERAGES(1) = 0.0 	AVERAGES(2) = 0.0 	SUMXY = 0.0   	SUMX2 = 0.0 	a = 0.0 	b = 0.0   	ON D% GOTO 1000, 2000, 3000   	 6 	! Print the values of the two fields for the current  	! collection.	   D 6000	CALL DTR$COMMAND (DAB BY REF,"FOR CURRENT PRINT !CMD, !CMD,", & 	 		  FIELD1$, FIELD2$)   < 	! Command DATATRIEVE to print the values calculated by the  	! equation.    	CALL DTR$COMMAND (DAB BY REF, &) 			  '!VAL + !VAL * !CMD ("ESTIMATE")', & # 			  A BY DESC, B BY DESC, FIELD1$)   * 	CALL DTR$COMMAND (DAB BY REF, " ON TT: ")) 	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)  	RETURN   A 	! If the user enters CTRL/Z, end the interface and exit program.   " 8000	IF ERR = 11% THEN RESUME 8200   8100				ON ERROR GOTO 0   - 8200	CALL DTR$DTR BY REF (DAB, DTR$M_OPT_CMD)  	CALL DTR$FINISH (DAB BY REF)    8300	END��                                                                                                                    �               * [C]MENU.FOR;1 +  , z   . 	    / s P   4 O   	    �                    - 
    0  � 1    2   3      K � P   W   O 	    5   6 ��R��"�  7  �+�*�  8          9          G s P  H �� J �                 K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C L C The program MENU shows you how to give users access to the DATATRIEVE dataL C management capabilities.  MENU enables users unfamiliar with DATATRIEVE to@ C display, store, modify, and report data managed by DATATRIEVE.N C MENU calls the subroutine MESSAGE to suppress the display of some DATATRIEVEL C messages and to display all other messages and print lines. The subroutine@ C MESSAGE is listed as an example of the DTR$CONTINUE command in: C the VAX DATATRIEVE Guide to Programming and Customizing.4 C In addition, MENU calls the following subroutines: C 	ESTABLISH
 C 	DISPLAY C 	SORT 	 C 	MODIFY 	 C 	REPORT  C 	STORE	 C 	CHOOSE K C**************************************************************************  C             M E N U . F O R  C & C Include the DATATRIEVE Access Block. C  	INCLUDE 'DTR$LIBRARY:DAB' C  C Declare variables. C  	CHARACTER*31 DOMAIN 	INTEGER*2    CHOICE 	INTEGER*2    COL/1/ 	INTEGER*2    LINE/1/  	INTEGER      STATUS   C Select initial options.   ) 	    INIT_OPTS =   DTR$K_SEMI_COLON_OPT   % 	1               + DTR$K_UNQUOTED_LIT % 	2               + DTR$K_FORMS_ENABLE  C + C Initialize the interface with DATATRIEVE.  C < 	STATUS = DTR$INIT (DAB, 100, MSG_BUFF, AUX_BUFF, INIT_OPTS)  ! 	IF (STATUS .NE. SS$_NORMAL) THEN 3 		WRITE (6, *) ' DATATRIEVE initialization failed.'  		STOP 	END IF    C Clear the screen.   " 10	CALL LIB$ERASE_PAGE (LINE, COL)  ; C Declare a port for the count of records in the collection  C the user establishes.   2 	CALL DTR$COMMAND (DAB, 'DECLARE PORT PT1 USING ')8         CALL DTR$COMMAND (DAB, '01 NUM PIC 9(4) COMP.;')" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  % C Prompt the user to choose a domain.    	CALL CHOOSE (DOMAIN)   / C Display the menu and prompt for a selection.     50	DO WHILE (.TRUE.)   	CHOICE = 0   2 	  DO WHILE ((CHOICE .LT. 1) .OR. (CHOICE .GT. 8))) 	      ISTAT = LIB$ERASE_PAGE (LINE, COL)  	      WRITE (6, 60)" 60	FORMAT (//23X,'MENU'/23X'----'/. 	1/10X,'1. Establish a collection of records.'+ 	2/10X,'2. Display the current collection.' ( 	3/10X,'3. Sort the current collection.'* 	4/10X,'4. Update the current collection.'* 	5/10X,'5. Report the current collection.'0 	6/10X,'6. Store records in the current domain.'# 	7/10X,'7. Choose another domain.'  " 	8/10X,'8. End this session.'/////. 	1/10X'    Enter the number of the operation '$ 	2/10X'    you wish to perform: ',$)   C Read in the user's choice.   	  READ (5,70)CHOICE 70	  FORMAT (I2)  	 	  END DO   ? C Call the appropriate subroutine to handle the choice entered.   + 	IF (CHOICE .EQ. 1) CALL ESTABLISH (DOMAIN)     	IF (CHOICE .EQ. 2) CALL DISPLAY  & 	IF (CHOICE .EQ. 3) CALL SORT (DOMAIN)  ( 	IF (CHOICE .EQ. 4) CALL MODIFY (DOMAIN)  ( 	IF (CHOICE .EQ. 5) CALL REPORT (DOMAIN)  ' 	IF (CHOICE .EQ. 6) CALL STORE (DOMAIN)    	IF (CHOICE .EQ. 7) THEN  2 	    CALL DTR$COMMAND (DAB, 'FINISH !CMD', DOMAIN)& 	    CALL DTR$DTR (DAB, DTR$M_OPT_CMD)$ 	    CALL LIB$ERASE_PAGE (LINE, COL) 	    CALL CHOOSE (DOMAIN)  	END IF   : C Finish the session with DATATRIEVE and stop the program.   	IF (CHOICE .EQ. 8) THEN   		CALL DTR$FINISH (DAB)  		GO TO 999  	END IF    	END DO    999	END ��                                                                                                                                                                                                                                                                                                                                        �               * [C]MESSAGE.FOR;1 +  , |   .     / s P   4 :       r  �               - 
    0  � 1    2   3      K � P   W   O     5   6 @wM�b�  7 ��+�*�  8          9          G s P  H �� J �               	SUBROUTINE MESSAGE  	INCLUDE 'DTR$LIBRARY:DAB'  : C While DATATRIEVE is at either the message or print line  C stallpoints, -  0 	DO WHILE ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .OR.$ 	1(DAB$W_STATE .EQ. DTR$K_STL_LINE))  / C if DATATRIEVE is at the message stallpoint, -   / 	    IF ((DAB$W_STATE .EQ. DTR$K_STL_MSG) .AND.   , C and the message is one of the following: -  3 	1 ((DAB$L_CONDITION .EQ. %LOC(DTR$_RECFOUND)) .OR. 3 	2 (DAB$L_CONDITION .EQ. %LOC(DTR$_ASSUMELIT)) .OR. 2 	3 (DAB$L_CONDITION .EQ. %LOC(DTR$_NONDIGIT)) .OR.3 	4 (DAB$L_CONDITION .EQ. %LOC(DTR$_SUCCESS)))) THEN   1 C use the call DTR$CONTINUE to skip the message;     		CALL DTR$CONTINUE (DAB)  	   ELSE  * C otherwise, print out the message buffer.  ( 		WRITE (6, *) MSG_BUFF(1:DAB$W_MSG_LEN)  " C Instruct DATATRIEVE to continue.   		CALL DTR$CONTINUE (DAB) 
 	   END IF 	END DO 
 40	RETURN  	END  ��                                                                                                                                            �               * [C]MODIFY.FOR;1 +  , }   .     / s P   4 O       �                    - 
    0  � 1    2   3      K � P   W   O     5   6 �;���"�  7 ���+�*�  8          9          G s P  H �� J �               K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C*****�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  2                        �! $      
DTRJA061.C                     }  
  [C]MODIFY.FOR;1                                                                                                                O                              �             *********************************************************************  C 9 C******************************************************** 9 C            SUBROUTINE MODIFY                          * B C Sets up a second menu and modifies records.           *         9 C This program is a subroutine of the MENU.FOR program  * 9 C********************************************************    	SUBROUTINE MODIFY (DOMAIN)  	INCLUDE 'DTR$LIBRARY:DAB' 	CHARACTER*80 ANSWER, CHLINE# 	CHARACTER*31 DOMAIN, FIELDS, VALUE 0 	CHARACTER*27 TEXT/'PRESS RETURN TO CONTINUE >'/ 	CHARACTER*1  CR 	INTEGER*4    NUMBER) 	INTEGER*2    ERASE_LINE/1/, ERASE_COL/1/ # 	INTEGER*2    P_LINE/24/, P_COL/15/  	INTEGER*2    IANSWER, NUM 	IANSWER = 1   	DO WHILE (IANSWER .NE. 3)   C Erase the screen.   3 400	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)        C Display the modify submenu.    	WRITE (6, 406) = 406	FORMAT (////'    1. One or more fields for one record.'/' @ 	1    2. One field for all records in the current collection.'/'9 	2    3. Return to main menu'///' Enter your choice: ',$)  	READ (5, 407) IANSWER 407	FORMAT (I1)   ; C Command DATATRIEVE to start with the first record in the   C current collection.   $ 	CALL DTR$COMMAND (DAB, 'SELECT 1;')" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	GO TO (410, 430, 499),IANSWER 	WRITE (6, 408) . 408	FORMAT (' Invalid operation...try again.')* 	CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL) 	READ (5, 401) CR  401	FORMAT(A) 
 	GO TO 400  0 C Select records one at a time. Prompt for which$ C records the user wishes to modify.  * 409	CALL DTR$COMMAND (DAB, 'SELECT NEXT;')   C Print any messages.   + 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  	END IF   / 410	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)    C Display the selected record.  $ 411	CALL DTR$COMMAND (DAB, 'PRINT;')" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  , C Inquire if this record needs modification.   	WRITE (6, 412) < 412	FORMAT (//' Is this the record you wish to update?'//'  ! 	1 Enter YES, NO, or EXIT : ', $)  	READ (5, 401)ANSWER> 	IF (ANSWER(1:1) .EQ. 'E' .OR. ANSWER(1:1) .EQ. 'e') GO TO 400; 	IF (ANSWER(1:1) .NE. 'Y' .AND. ANSWER(1:1) .NE. 'y') THEN   		NUMBER = NUMBER +1 		GO TO 409  	END IF , 	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)  ( C Show the fields available to the user.  6 413	CALL DTR$COMMAND (DAB, 'SHOW FIELDS !CMD' ,DOMAIN)" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	WRITE (6, 415) 6 415	FORMAT (' Which field do you wish to modify: ', $) 	READ (5, 401)FIELDS   C Modify and check for errors.  / 	CALL DTR$COMMAND (DAB, 'MODIFY !CMD;', FIELDS)   + 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  		WRITE (6, 418), 418	FORMAT (' DO YOU WANT TO TRY AGAIN? ',$) 		READ (5, 401)ANSWER		 4 		IF ((ANSWER .EQ. 'N') .OR. (ANSWER .EQ. 'n')) THEN 		    GO TO 400  		ELSE 		    GO TO 411  		END IF 	END IF    C Print any messages.   " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD), 	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)  * C Print the modified record on the screen.  ! 	CALL DTR$COMMAND (DAB, 'PRINT;') " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	WRITE (6, 423) 4 423	FORMAT (/' Do you wish to modify any more fields 	1 in this record? ', $)   	READ (5, 401) ANSWER > 	IF (ANSWER(1:1) .EQ. 'Y' .OR. ANSWER(1:1) .EQ. 'y') GO TO 413 	WRITE (6, 425) : 425	FORMAT (/' Do you wish to continue updating records?') 	READ (5, 401) ANSWER 9 	IF (ANSWER(1:1) .EQ. 'Y' .OR. ANSWER(1:1) .EQ. 'y') THEN  		NUMBER = NUMBER +1 		GO TO 409  	END IF 
 	GO TO 400  A C Modify one field for all the records in the current collection.   / 430	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)  432	WRITE (6, 435)C 435	FORMAT (/////' Do you want to:'//'    1. Update all records to  > 	1 one value.'/'    2. Update all records with an equation '/'6 	2           (for example, price = price + 400)'/'    $ 	33. Return to the previous menu. '/ 	4/' Enter 1, 2 or 3: '$)  	READ (5, 440)NUM  440	FORMAT (I) 	GO TO (460, 445, 400), NUM    C Prompt for an equation.   / 445	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)    C Show the fields available.  & 	CALL DTR$COMMAND (DAB, 'SHOW FIELDS')" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	WRITE (6, 450) 2 450	FORMAT (' Enter the equation you wish to use') 	READ (5, 401) CHLINE   / C Update all records in the current collection.   9 	CALL DTR$COMMAND (DAB, 'MODIFY ALL USING !CMD;', CHLINE)    C Check for errors.   + 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  	END IF   5 C If not successful, show the fields and start again.   2 	IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN 		GO TO 430  	END IF , 	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL) 	WRITE (6, 455) $ 455	FORMAT (' All records updated.')   C Print the updated collection.   ) 	CALL DTR$COMMAND (DAB, 'PRINT CURRENT;') " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)* 	CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL) 	READ (5, 401) CR 
 	GO TO 400  # C Modify one field for all records.    / 460	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL) & 	CALL DTR$COMMAND (DAB, 'SHOW FIELDS')" 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	WRITE (6, 465) 6 465	FORMAT (' Which field do you wish to update? ', $) 	READ (5, 401) FIELDS   + C Issue the MODIFY statement to DATATRIEVE.   > 	CALL DTR$COMMAND (DAB, 'MODIFY ALL !CMD OF CURRENT;', FIELDS)+ 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  	END IF   ( C Check to see if MODIFY was successful.  2 	IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN' 		CALL DTR$COMMAND (DAB, 'SHOW FIELDS') # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  		GO TO 430  	END IF    C Prompt for a value.    475	WRITE (6, 477) MSG_BUFF % 477 	FORMAT (1X, A<DAB$W_MSG_LEN>, $)  	READ (5, 401) VALUE   C Pass the value to DATATRIEVE.     	CALL DTR$PUT_VALUE (DAB, VALUE)+ 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN # 		CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  	END IF 2 	IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN 		WRITE (6, 478)) 478		FORMAT (' Invalid, try again....'//)  		GO TO 430  	END IF , 	CALL LIB$ERASE_PAGE (ERASE_LINE, ERASE_COL)    C Print the modified collection.  ) 	CALL DTR$COMMAND (DAB, 'PRINT CURRENT;') " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)* 	CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL) 	READ (5,401) CR   	END DO   
 499	RETURN   	END��                                                                                                                                                                                                                                                                                                                                    �               * [C]PARSE.FOR;1 +  , �   .     / s P   4 M                           - 
    0  � 1    2   3      K � P   W   O     5   6 �%ɛ�"�  7  �+�*�  8          9          G s P  H �� J �                M C**************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C ( C  $ FORTRAN CORRELATE, PARSE, CALCULATE. C  $ LINK CORRELATE, PARSE, CALCULATE, DTR/OPT C  $ RUN CORRELATE C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.M C****************************************************************************  C C C****************************************************************** B C                    SUBROUTINE PARSE 	                          *C C Parses the arguments to the CORRELATE statement, converts the   * C C statement into a BEGIN-END block, and passes the block to       * C C DATATRIEVE for processing.                                      * C C This program is a subroutine of the CORRELATE.FOR program       * C C******************************************************************    C 8 C   Declare variables and include definition of the DAB. C  	SUBROUTINE   PARSE()    	INCLUDE      'DTR$LIBRARY:DAB' & 	INCLUDE      'DTR$LIBRARY:COMMON.FOR' 	CHARACTER*33 FLD1   	CHARACTER*33 FLD2   	INTEGER*4    DTR$GET_STRING 	INTEGER      RET_STATUS 	INTEGER      COMMA_LENGTH 	INTEGER      OF_LENGTH  	INTEGER      NUM_TOKENS C  C Declare the status messages. C  	EXTERNAL  DTR$_ENDOFSTR 	EXTERNAL  DTR$_MORESTR  	EXTERNAL  SS$_NORMAL  C B C  Set the variable NUM_TOKENS to the number of to�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  3                        J<} $      
DTRJA061.C                     �  
  [C]PARSE.FOR;1                                                                                                                 M                              ��              kens needed (3).1 C  Set token length variables for the test_token.  C  	NUM_TOKENS = 3  	COMMA_LENGTH = 1  	OF_LENGTH = 2 C D C Use the DTR$GET_STRING call to get the first token. This should be$ C the name of the independent field. C G C If there was no token, DATATRIEVE returns the status (DTR$_ENDOFSTR). F C If this status was returned, set the number of tokens received to 0. C 4 	RET_STATUS = DTR$GET_STRING (DAB, DTR$K_TOK_TOKEN,  	1       FIELD1). 	IF (RET_STATUS .EQ. %LOC(DTR$_ENDOFSTR)) THEN 		NUM_TOKENS = 0 	END IF  C 9 C If there was a token for FIELD1, DATATRIEVE returns the > C status DTR$_MORESTR. If this was returned, then make another& C DTR$GET_STRING call to DATATRIEVE.   C , C The next token may be the optional comma. - C Make the call using DTR$K_TOK_TEST_TOKEN.    C 5 C If the token length is 1 and the token is a comma,  7 C DATATRIEVE places the token in the field named COMMA. ; C Otherwise, DATATRIEVE returns a status of DTR$_MORESTR.    C : C If there was no token, DATATRIEVE returns DTR$_ENDOFSTR./ C In this case, set the number of tokens to 1.   C - 	IF (RET_STATUS .EQ. %LOC(DTR$_MORESTR)) THEN = 	    RET_STATUS = DTR$GET_STRING( DAB, DTR$K_TOK_TEST_TOKEN,  ! 	1      COMMA, COMMA_LENGTH, ',') 3 	     IF (RET_STATUS .EQ. %LOC(DTR$_ENDOFSTR) )THEN  		NUM_TOKENS = 1 	     END IF 	END IF  C F C If DTR$_MORESTR was returned, then make another DTR$GET_STRING call.8 C The next token should be the dependent field (FIELD2). C E C If the status is DTR$_ENDOFSTR, then there was no token for FIELD2. @ C Set the number of tokens to 1. (The number of tokens does not $ C include the comma or the word OF). C     - 	IF (RET_STATUS .EQ. %LOC(DTR$_MORESTR)) THEN 9 	     RET_STATUS = DTR$GET_STRING( DAB, DTR$K_TOK_TOKEN,   	1      FIELD2 )3 	     IF (RET_STATUS .EQ. %LOC(DTR$_ENDOFSTR) )THEN  		NUM_TOKENS = 1 	     END IF 	END IF  C = C If the status is DTR$_MORESTR, make another DTR$GET_STRING  9 C call. The next token may be the optional word OF. Make  ( C the call using DTR$K_TOK_TEST_TOKEN.   C A C If the token length is 2 and the token is OF, DATATRIEVE places A C the token in the field named WORD_OF; otherwise, it returns the  C status DTR$_MORESTR.   C : C If there was no token, DATATRIEVE returns DTR$_ENDOFSTR.5 C In this case, the program sets the number of tokens * C to 3 and sets the last token to CURRENT.- C CORRELATE then uses the CURRENT collection.  C - 	IF (RET_STATUS .EQ. %LOC(DTR$_MORESTR)) THEN = 	    RET_STATUS = DTR$GET_STRING( dab, DTR$K_TOK_TEST_TOKEN,  ! 	1      WORD_OF, OF_LENGTH, 'OF') 4 	    IF ((RET_STATUS .EQ. %LOC(DTR$_ENDOFSTR)) .OR.  	2	 (OF_LENGTH .EQ. 0) )THEN 		EXPRESSION = 'CURRENT' 		NUM_TOKENS = 3 	    END IF  	END IF  C ; C If DTR$_MORESTR was returned, make another DTR$GET_STRING  C call.  C ; C The rest of the line the user entered should be a record  = C selection expression. Use the DTR$K_TOK_COMMAND token type  0 C to place the remaining tokens in EXPRESSION.   C > C If the status is DTR$_ENDOFSTR, no RSE was entered. Set the  C number of tokens to 2.   C     - 	IF (RET_STATUS .EQ. %LOC(DTR$_MORESTR)) THEN : 	    RET_STATUS = DTR$GET_STRING( dab, DTR$K_TOK_COMMAND,  	1	EXPRESSION ) 2 	    IF (RET_STATUS .EQ. %LOC(DTR$_ENDOFSTR) )THEN 		NUM_TOKENS = 2 	    END IF  	END IF  C , C    (The user entered DTR> CORRELATE <RET>) C = C If the number of tokens is 0, then prompt the user for the   C name of an independent field.  C  	IF (NUM_TOKENS .LT. 1) THEN, 	    WRITE (6, *)'ENTER INDEPENDENT FIELD: ' 	    READ (5, 110) FIELD1  110	FORMAT(A)  	END IF  C 4 C    (The user entered DTR> CORRELATE field1<RET> or3 C                      DTR> CORRELATE field1,<RET>)  C > C If the number of tokens is 0 or 1, then prompt the user for   C the name of a dependent field. C  	IF (NUM_TOKENS .LT. 2) THEN* 	    WRITE (6, *)'ENTER DEPENDENT FIELD: ' 	    READ (5, 110) FIELD2  	END IF  C < C    (The user entered DTR> CORRELATE field1 field2 OF<RET>) C > C If the number of tokens is 0, 1, or 2, then prompt the user  C for an RSE.  C  	IF (NUM_TOKENS .LT. 3) THEN8 	    WRITE (6, *)'ENTER A RECORD SELECTION EXPRESSION: ' 	    READ (5, 110) EXPRESSION / 	    IF (INDEX(EXPRESSION,'     ') .EQ. 1) THEN  		EXPRESSION = 'CURRENT' 	    END IF  	END IF  C G C Use a library routine to find the first blank space in the variables  C C for the field names. Because field names cannot contain blanks,   E C this will return the length of the field name along with one space.  C B C Store the field names with quotation marks around them into the - C temporary storage variables, FLD1 and FLD2.  C  200	LEN = LIB$LOCC(' ',FIELD1) 	FLD1 = '"'//FIELD1(1:LEN)//'"'  	LEN = LIB$LOCC(' ',FIELD2)  	FLD2 = '"'//FIELD2(1:LEN)//'"'  C 6 C Change the INIT_OPTIONS to just DTR$K_MORE_COMMANDS.; C This allows the program to pass all of the command lines  6 C before DATATRIEVE starts checking for syntax errors. C $ 	DAB$L_OPTIONS = DTR$K_MORE_COMMANDS C 3 C Pass DATATRIEVE all of the statements required to 7 C retrieve and process the data for a linear regression 0 C equation and the coefficient of correlation.   C 5 C All statements should be part of a BEGIN-END block.  C   	CALL DTR$COMMAND (DAB, 'BEGIN') C 9 C Declare the ports that will be needed.  These ports are 0 C deleted at the end of the BEGIN-END statement. C 3 C Port PT1 is for the number of records to be used. ; C Port PT2 is for the total of the fields in the collection , C and for the individual values per record.  C 2 	CALL DTR$COMMAND (DAB, 'DECLARE PORT PT1 USING ')<         CALL DTR$COMMAND (DAB, '    01 NUM PIC 9(4) COMP.;')2 	CALL DTR$COMMAND (DAB, 'DECLARE PORT PT2 USING ')( 	CALL DTR$COMMAND (DAB, '    01 WHOLE.')2 	CALL DTR$COMMAND (DAB, '       02 PART_A REAL.') 9         CALL DTR$COMMAND (DAB, '       02 PART_B REAL.;')  C = C Store the total number of records in the RSE into port PT1.  C          CALL DTR$COMMAND (DAB,  6 	1	'STORE PT1 USING NUM = COUNT OF !CMD;', EXPRESSION) C : C Store the total of both fields in the RSE into port PT2. C * 	CALL DTR$COMMAND (DAB, 'STORE PT2 USING')$ 	CALL DTR$COMMAND (DAB, '    BEGIN'): 	CALL DTR$COMMAND (DAB, '    PART_A= TOTAL !CMD OF !CMD;', 	1	FIELD1, EXPRESSION): 	CALL DTR$COMMAND (DAB, '    PART_B= TOTAL !CMD OF !CMD;', 	1	FIELD2, EXPRESSION)# 	CALL DTR$COMMAND (DAB, '    END;')  C @ C One at a time, store the value of both fields for each record  C into port PT2  C . 	CALL DTR$COMMAND (DAB, 'FOR !CMD',EXPRESSION)# 	CALL DTR$COMMAND (DAB, '  BEGIN ') / 	CALL DTR$COMMAND (DAB, '    STORE PT2 USING ') ( 	CALL DTR$COMMAND (DAB, '        BEGIN')8 	CALL DTR$COMMAND (DAB, '        PART_A= !CMD;', FIELD1)8 	CALL DTR$COMMAND (DAB, '        PART_B= !CMD;', FIELD2)' 	CALL DTR$COMMAND (DAB, '        END;')   	CALL DTR$COMMAND (DAB, '  END') C : C After all records in the RSE have been stored into PT2, 8 C instruct DTR to display the names of both fields used. C This serves two purposes:  C A C 1. The subroutine CALCULATE knows when DATATRIEVE has finished  & C    storing the records in the RSE.   C B C 2. DATATRIEVE passes the names of the fields to CALCULATE in theD C    auxiliary buffer, and the length of the names in DAB$W_AUX_LEN.9 C    The field names are saved if more than one CORRELATE / C    statement is entered within one statement.  C . 	CALL DTR$COMMAND (DAB, 'DISPLAY !CMD;', FLD1). 	CALL DTR$COMMAND (DAB, 'DISPLAY !CMD;', FLD2) C H C Change the options back to the INIT_OPTIONS declared in the INIT call. C  	DAB$L_OPTIONS = INIT_OPTS C  C End the BEGIN_END block. C  	CALL DTR$COMMAND (DAB, 'END;')  C  C Return to the main program.  C  	RETURN  	END��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  �               * [C]PAYROLL.COB;1 +  , �   .     / s P   4 N       d                   - 
    0  � 1    2   3      K � P   W   O     5   6  ���"�  7 @j�+�*�  8          9          G s P  H �� J � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  4                        xPf� $      
DTRJA061.C                     �  
  [C]]L_W,W)LuNCG;1                                                                                                             �  ��                         ���<     �       ��������􃋻��ҩʖ\����!UI��	��Z��͖GHF���X+#2C��븑����ǉ��iEY)"4���W<xziN .OaSAH,D!Cmwn7-_jVd#�Vt=eNu	\
��䟰���������������㲧�䈨�冭�)'Q9 C(hh#��(�U�����	!�0�:�:x̽��̙��OpI��f~E"US9 Q^*���_uM0I�	J!vVu/1Qq| Finh�W#$%Ags����������İ�����˔�ۓ�r_驤�ǳ��N
9Tgsf����������������ʸ�ײ�>!z����rUlw&y����Q4WT�އ�����XPJUOJ�������ׯ����ˏ���������ڤ�{Q4ㆠ�����Ўζ�������.eBNRur3U9?)����4Q@^IU͞�������ц��Ȩ����>r??!Hd8s�������������ʥ�㨗���������9^&"���Ň�@�)1H��Bi���ƼJx���Kf\.Sahw026:Qh��߼��4W$x ���߇��Xgu^½���������ۗ���Ϻ�����������dp7HXXw#[T��������/v<hU���ژ�C;uo�����Og1~-hS'FWtĶ�祅�i4g���苄������S1{����cv<x6і���Դ�����2xVQ���������fh7rmi#V<:_���+6��52/?Q_>%
lH���꘵ҙ�ꜚ�����������*	b72eQ~,2D3u#V**;gzzZLKs����׹o.��˘�
K06A����yNPG6��]tYZZ`�Ke;gkCf0HA| xX]x��鏶ӥ�J���ߪ�Į���ؠ���M̔�����������+U `@aKwI4K-EIoш�����|:_賃lZ,FG&tj6n4Y-+_BKp_�}s\|qgvsX;{K-U.,V�	GD8u"MN\E|8oD`DK$E>:C$RD26 X/k<1F8QxGv!d3�:WY(FB��η��d������ء�����m��Ŝ����F$|�����"*6`"w{:I4��ь�H(_/plD8 /v������8W%�dpvy ��Y&^GF'mmNtO+$��Ǚ��٨�ߢ�C������1�f�?����Ĉ�������������.ilbAd��������1���4i+V7HgPi	x�А���둞^|6
b(i35tv99uspUs{kD|xj����0�����ù�Õ��C��ƫ���ַ����ǃ���������}i/jx����2fkuH����/WhSU����������LjaDMLOs'! IFt{j%yCH0Vd������������=P.t���ب��O4`39GG��o7K�¿�������dJELY�9��ڍ�#P��L� 0cY������ׅ�ڕӕ칑������«�F�˾��*5$wDjIFqg0(.XMjKHnß��%?(w?�������⨓8+z6-- W6Itt2><�ԓ��������Y�=sʂ�<y0q]j0w.'�������ѐ���x��p q'gWwn$KU^\GeQq}%�A)&=f}^fGAA69����\0 Pd'r$�gՀ��j̩��Y��?�!��XY���������-fCae]1v ^7N\[J	s ��i0[�me*ba1IE|<VnE3,)!$<^a8l2AdN89vZ~;~1Bv{~J(\lj`Vwbyr1GQw(s`7:Bd<Rq!TPk0N7٥:%q��Ȧ��&Vb9�����(ɕ篾������;	>x���r@I@<4aG3/jD$I���H=��̂K!cjV:}[t9n������⸻���gp(Wc><-YT��Ҍ�ս�Ƣ��ڒ����������5c).AxFm?-0]e~�����ʂ�����ћ�ț����ȳ���f��M0`Hg68P7hf-Q��������������������Ɯ�PM}T=�X.C>~&=EoeZۮ��,l`\:6jvYx~opI?<�#n�J"1�(rGP]UwRjT*gLLV=	]߱�߆���\��ꡈ�>���ǹ��͎�����������L=���㈻���t$w]U#/`ed5
��U��.���g�	�7������룇������[���D(aNMe'����Ls��c�
lE/de{=lWpW@[4欤�����^VMCa55A]**/Os:]\:S/:222;ƿ�٪��٤��ͧ������������蜡���K!aoy|[,B]~U!t9i"3+Fo#c'P3>~$k G i Ux}ğ��ݠ"T`�1W������~Rzb��SL;.DVeb	l
sD�������������ۣ��Ŧ�4s#,4V	`!`Z]x8_=HX&Ofh:"&*eQC~k9=&TNj~m/y8"0	:vy)1sBt�^%4l\HEl&`vR<��Ҭ��L��۷���ᢂ,\Â��ʋ�����)Ѻ7�<f������ս�㪩��Ѵ�ņۍ����̜�?�Y���˰����Q(g麗�����ID̴������Q�nZ�\7f�����5!VE7�E�;>UH� GG\_������򿓾�Y�˷��υ�(a��$��e��V�iIOhq#"j2XV�\�1�fu��N7f# 70��"uH��l	,�~d4.B'.tr%j@j -e'L]=<O)n\F)?T3V�a� }{QUg;0Lx''K{J+Rc$1!DIQ���ʳ�ڸ�0�|�l�5�~�{y�U(a~)DS[�����oQ I!;P�6Kt?Z<F,q7ik)%���,+;(W>w_p5N8A(y!B0PoEK%6PK7z	tUJVyTfsKxR?y)w=	g X+b:~Z:XwbH	k%dV`4H>L*6UF_|iicKҮ��Ɩ�ai���������и��������⌆����ٺ�v]`U.Ӧ?dOVHnxꊬ��ޅ����ڃ���������ŭ����ّ�����"OZ`VoOEIpyI*R6%q3#sZobA?��嘌����ڞ������ɢ���ŏ�Ǽ����ʩGRg[S9KTm|9"Pt= z�����&O������6,xn;_��ד�kVU+DĶý������oyM*Z?G����Ñ��͗ހǴ����Դ΃�����������Ӳ�luD,- 4xnT߃��������񥬯#{U9Eԯ����F�k"U-I(*5W``\����̑qT=!ifI=	<2G����_�;3���^-,r1�Ⱦ�����ʉ��i>t+J������2P8>)<Cl,pR,d�������Ϋ��ٻ�����,9V=Q!}QԱ��y ������1)m����NA:G`j:q\lC0Q@B<5[u]#si��x"��ܯ��⮘��������¦Ze����KfMp���ܜ�����9+ri<������dN|y3'
-��ĺ��咯ڌ�����������oVyQDן�֎��$
3qJE?C\x��uMX}j�����ħ�񵮟�ٮ������CjOZ�������՚��߀�댬�⠯�������ߎ���՗��Ж��?0a����ߋ�mu]5 W|;[�L�iֿ����:3z*A<L��ͫ��b���ؗ��E40�ؿ��̡��.kJ)u���0IA}egn_JaIuHiEZrgR$hrh8@CqtNjyoVv\' u`m*iHtC@G'3
s(4<vbJoK:-"MvCL&;xkoW�آ�Ǿ��͡�^D	4�[#)Y&DF;G=  yb%UNq�ʛ�3ul]_Cv[
Rp2r=��d]I���������/0nJYU|&bMcW^)f}`'	+zR1А��򿆍�ݠ��ħ��sI1mVX!I�ҡAbA����������꛱���ʕ���ǿ������������G8>w?,(��Xni{h�໭�MSkh&fc����¿������)$$%`�����������Q�����,Eu>u+)^@/SUTXPj6 <k*����Ų�����Dn@!p�ƪ�����Ďɒ���ج$!HWᘂʽ�����E@K���4q6��;����}jWGNY(mZq$q|%-1"\W*+!KZ
��,VlO��uq	�􎻅�����LJ9>vj�ߘ���r1`�����Œ��������ˀ����ƞ
#zw}iQ+*j{H���=Rq%c�L2_*������������ߍBQ@BH(y(G{w+^��A2�k'#b%����ʊ�1����ʴ��@$��Ǣ�����ت__}udsx41iyJvKjI]lNf+@5��ދ������������gZ!LDW]DE#(+X([l9sPG75Gb(fR������ﶮ�������/�����������������1!R/z!�ͤ��τ��ANT	N8Խ�����������Е����޳����������	~q�V};Rj IW,K{t9XCbG-FgZR�K�������Ź��m8��i&Tj,c|�u.p&�'VWy�����Y\RO	4Iup����x%�Xy��tO�F�~H].+	<.1M_@xan0@-��?���@f,\zR8fYh��$<w�@2~{wylfEw��������$�芇��¿���EIh;^�_cnp|"nb<9<f3l#߉����f5?un�����ؼ��������������#yDsoj/\nN:��Ƅ��������Ġ������̔�����W	kN@c���������҃���<վ��忤���������񔀤ňpHrzP%+4��˧^��Ԑ����#�M�NM{Pv芙��烐��zT,G^U:YD2F}O�J�{�$'*�Ȭ��ظ������'��_- -E�����������nm8Jl�񑮾���$sz$#	#1&�β���Ҵ҈̥���߽�ݖ��+l_uQ\9����00@hE1�ݿ)(|!	>tI�ԋ����
|MNBt�IG8=J��ۦ�鷻�����ۼ��ԡ��ъ�I901X<
4t8)�BL�G�E+j��S�]�a{zyDB`b{_1X}r����2 	B}iJ/[,C��L����AP~H(g�������ٱP\:C#�������KkӋ�����-h�ܷ���foh Vj\=zVdsc-^7 RuT#�����Qmq;0VrYz7OtbgE�B}W^L)��󚰋��ͧ�̓���н������1�nB6T���:>+IQ[3f�בgͅ�ѫ���K=*M2Hݼ��"B�b�/|O�8x�̡�}�q���=y_\{up4LEM`^��{ζ�{	&#�!?%��
p΁������֝��8 1p[a��������Sc�M�o��ޯ�����µٹ����cC6C.>5yӷ�Tb��%:g8WGd$)4:,t:̑�𻦂������������ý����9bA2jyxJ
#	]l[5ip`J{[d_65^~QX�.[#��݁߶����������������������om6+F-9R+ZW5}h	>+-x���pMetX��t@W)*ӷ嬧���ۦ젯���쩸�2zR `ܔ�����Ёԑ�����̀������q{2v=2*j8z6z/[d76��\3Uk������慔���؆�����JZan4#-n?^�� "/���������o$d9��ο�w �љ�ޘ��A4jKj4X�����Ł���#�^�����������zctL�ѷ������������Ӄ������Й�,*Dj*��FfQ\~EV`WtBVkxu<:0-^l2d{n;=S��Ǿ���������䕵���m8w cA������������̼͑������˄���ӏ����&WXQ)0nas
t0A6MED.0Zg[g{Zp;`{�ݡ��ȴ����5<�2
Hf!shuyIdv�����������A�⩸Qi��������9u],vuXTtΉ�M ږ瀸����<nN-3gFQSE*|>?d�.=�~4�����rT:>-gh��Β���л����v^5��A���F)|w
#s_B<��o����0F���ɋ������ʚ��͕��
�����՘��؎&gnQ'M����I~L+~M>g]^+hdGS_��ǥ��<A��ʴ���ڸ�������t�������9�p��W��уW! \_��������͡�Z	k �U"xT9b&FHGi<=gVq"tt�ljhU~Yj	�����+ 6b2;0��ڗ�a�����z��4e�;����ɨ�����������5J��q�ޫ��������f����j������؏�������`�ʃ�������������ܽ���ȍ϶���^+.BzX0*,{cl"}�&Z P�ڂ��ڰ�Ĕƌ��2ķ� �����ʊ����̨�����Л�CK����s#*R|H�J�̞���Ԟ��_��ϧ�ɦ�㿒����ʛ��X]e�W4WOwlK<||SH@4'Ht
cO}a�>��E�iU+>�a�1���ٰ˥��ګ�����ɪ��?M:m^��ŕ�ۥ�����WIlb�w*i[I.Y)c@S;ZEcK.|-eۋ�R?a��Ƶ̜�y#Jo8*bW5A+l�S��Գ�Ƈ���MY`))س������ޕ��������Ө�����˴���NWSJ,*`2[xX\;C*vCqG
0p+jmU�ܠ�$*s4q�⅛�HY�����̢��B����ۆ�ڑ��!Cu$	CzZ_z@>*D-7W<}L%(Fua-Hg��������zuBDt*B���՛��ޝ�説ͻ�ݭ��Ȏ�����`��a֐�5w��Ѓ�R9
dg(O;_F}VB*˭��������㔴PO8|_Ctp$����}&~AG#�������ӿ����dT6W1l/#^edEp~3_WG*E)|&qi?;kŪ��`FM&5-(�µϽ����������ɂ�{ 9?����nZS��ܚ�������BkRcd+A"z8~6-V����Ƌ����T# 
;=5K]<:ua������������Ϙ�ސʶ���%�����ڋ����W9YZ�ɷ�l(qQ\����������ո���"As!l'��骓�	nl5bbNA���������̤e.��������ꔰ��ԃ�������Õ�Ô��^Q��sLnE4t2#�������������N��kb������MKP.<L���U>$������9*hK*L5����>g��ׁ�����Ƽј������ϫ��詋������'AFWbJ)K[}dgp=&d6:eLQ
�t-,V?	l^+kN&�����韡��͟{߰
]\�ۍƾ���_R`8 �)<��CfOU{Ie!hV	*U���;�n����ມ�쭍�ޞ�����������ٙ����np�J-hk{{\ 'tTIucy�~ YB~8nH>-`,ksGJLcV$9XK/�j���z�)B�����W�U�:rheeBNeZ+NbT����d�؊������є����*��R(	7�dY{)؊�{@aN�ֽ��ˉ�������������������Y{~}�c����$U\�c��Ǘ�ּ�-fr3:m$TEplP7k8bF4Rnh~%\��ۄ���ґ��3uOM�������܄���ҙ����R1L@	}htn@>jQRM>'9N|XTY0 :aH:9w;Z`%Byj2o%ښƉK%oi(vo�������Bn`?kF};��ꛂ�����Ŕ��i�������פ��XP=`��ڻ�S7X�nS$c.24 }2r~ZE_%+c~
$LzmM.dthOyw$KC&x�vs=t,(gPcc�
F,qNh��bq\]؞����]I5{~	�������إi9[55y){v?L�������z=K '>n��0"-k41 3�݄�/n�M�ę�ќ"Plz��ڠ���HM��u�s.	\�$��j" Mv%KgKSKK��qJqW%%W!C`sLrB��3yp2���I���W`2 *] ,�ߑ����܆����(����뫃Ӵ��PgxP`@���^l`�髌˅����+3& N�F|M3`3�µ���쎧����_xiSj]������ڱؐ��;` e,d,0�He]kwZ6Bk��ϡæ|!������㋵_N�s%!QS!2~qR^N$SqRPY
�b*(����_�C;�v�3JPLcSjVe|| I|H³���?Tj3^qD7P8�2��̫3��D|�^:QE_Cc&)S�                        ~
lx
3++,,eT..  //ssPP  44 HJ    
��   �              --
  00 ��11  22 33    KK��~PP WW OO  5566 ��{��77 ����ͽ�88        99@���� GGssPPHH�"�JJ��       5                        xv�9 $      
DTRJA061.C                     �  
  [C]PAYROLL.COB;1                                                                                                               N                              l� "            M ***************************************************************************** M * To run this program, you must compile it and link it with the DATATRIEVE  * M * shareable image:                                                          * M *                                                                           * M *  $ COBOL PAYROLL                                                          * M *  $ LINK PAYROLL, DTR/OPT                                                  * M *  $ RUN PAYROLL                                                            * M *                                                                           * M * See the VAX DATATRIEVE Guide to Programming and Customizing for more      * M * information on using an option file to link programs that call DATATRIEVE.* M *****************************************************************************  * M * The program PAYROLL reads data from a file and creates two other files. The M * program calls DATATRIEVE to get employee information stored in a DATATRIEVE  * domain, HOURLY_LABOR.  * M * The program uses the data files TIMECARD.DAT and LABOR.DAT. These files are I * not included with the sample material in the DATATRIEVE kit. To use the K * sample payroll and update applications, define the two files and populate  * them with sample data.D * The domain and record definitions for HOURLY_LABOR are as follows: * : * DOMAIN HOURLY_LABOR USING HOURLY_LABOR_REC ON LABOR.DAT; *  * RECORD HOURLY_LABOR_REC USING  * 01  PERSON. * *     05 ID                   PIC IS 9(5).1 *     05 EMPLOYEE_NAME        QUERY_NAME IS NAME. . *         10 FIRST_NAME           PIC IS X(10)7 *                                 QUERY_NAME IS F_NAME. ( *	  10 LAST_NAME            PIC IS X(10)1 *	                          QUERY_NAME IS L_NAME. ) *     05 DEPT                 PIC IS XXX. + *     05 START_DATE           USAGE IS QUAD 7 *                             DEFAULT VALUE IS "TODAY". * *     05 HOURLY_RATE          PIC IS 99.994 *                             EDIT_STRING IS $$$.99.) *     05 SUP_ID               PIC IS 9(5) 1 *                             MISSING VALUE IS 0.  * ;  * J * The program uses a port to read in records from DATATRIEVE. The port has * the following definition:  * + * PORT H_LABOR_PORT USING HOURLY_LABOR_REC;  * ' * The PAYROLL program works as follows:  * % * 1. It reads data from TIMECARD.DAT.  * M * 2. It uses H_LABOR_PORT to pass records from DATATRIEVE to a record buffer.  * N * 3. It writes production data from TIMECARD.DAT out to the file FINISHED.DAT. * @ * 4. It uses data from the DATATRIEVE record and TIMECARD.DAT to* *    calculate the weekly employee salary. * G * 5. Salary and other employee data are written to the file PAYROLL.LOG  *    and to the screen.  *         	 M *****************************************************************************  IDENTIFICATION DIVISION. PROGRAM-ID.            PAYROLL.   4 ****************************************************4 * The program reads data from a sequential file,   *4 * TIMECARD.DAT. The program then uses information  *4 * from a DATATRIEVE domain to create 2 log files:  *4 * PAYROLL.LOG and FINISHED.DAT.                    *4 ****************************************************   ENVIRONMENT DIVISION.  CONFIGURATION SECTION. SOURCE-COMPUTER.    VAX-11.  OBJECT-COMPUTER.    VAX-11.    INPUT-OUTPUT SECTION.    FILE-CONTROL. .     SELECT FINISHED-GOODS ASSIGN TO "FINISHED"'         FILE STATUS IS FNSH-GDS-STATUS.   3     SELECT PAYROLL-LOG-FILE ASSIGN TO "PAYROLL.LOG"  	FILE STATUS IS PAYROLL-STATUS.   .     SELECT TIME-CARD-FILE ASSIGN TO "TIMECARD"#         FILE STATUS IS TIME-STATUS.      DATA DIVISION. FILE SECTION.    FD  FINISHED-GOODS.  01  FINISHED-REC. "     03  F-PRODUCT-NUMBER	PIC X(9).     03  FILLER			PIC XX.&     03  F-JOB-HRS          	PIC 999V9.'     03  FILLER                  PIC XX. ,     03  F-JOB-COST              PIC 9(4)V99.   FD  PAYROLL-LOG-FILE.  01  PAY-REC.#     03  P-EMPLOYEE-NUMBER	PIC 9(6).      03  FILLER			PIC XXX. #     03  P-EMPLOYEE-NAME		PIC X(20).      03  FILLER			PIC XXX.      03  P-DEPT  		PIC XXX.     03  FILLER			PIC XXX. !     03  P-GROSS-PAY		PIC Z999V99.    FD  TIME-CARD-FILE8     RECORD IS VARYING IN SIZE FROM 18 TO 117 CHARACTERS. 01  TIME-REC. (     03  T-EMPLOYEE-NUMBER      PIC 9(5)."     03  T-JOB-COUNT	       PIC 99.(     03  T-JOB-INFO  OCCURS 1 TO 10 TIMES 	    DEPENDING ON T-JOB-COUNT.,         05  T-PRODUCT-NUMBER       PIC X(9).*         05  T-PRODUCT-HRS          PIC 99.   WORKING-STORAGE SECTION.4 01  TIME-STATUS                PIC XX  VALUE SPACES.4 01  FNSH-GDS-STATUS            PIC XX  VALUE SPACES./ 01  PAYROLL-STATUS	       PIC XX  VALUE SPACES.   9 01  SUB1                       PIC 999 COMP VALUE ZEROES.  01  TOTAL-HOURS		       PIC 99. $ 01  OVERTIME-PAY	       PIC 9999V99." 01  GROSS-PAY		       PIC 9999V99. 01  COUNTER		       PIC 99.   * 01  LINENO                     PIC 9 COMP.* 01  COLNO                      PIC 9 COMP.B 01  WS-TEXT                    PIC X(16) VALUE '         WORKING'.  6 ******************************************************6 * Set up a record buffer by copying the CDD record   *6 * definition.                                        *6 ******************************************************  ( COPY "HOURLY_LABOR_REC" FROM DICTIONARY.  ) ***************************************** ) * Copy in the DATATRIEVE Access Block.  * ) *****************************************     COPY "DTR$LIBRARY:DAB.LIB".   01  DTR_OPTIONS PIC 9(9) COMP.% 01  DTR_RETURN_STATUS PIC S9(9) COMP.   % 01  STACK-SIZE PIC 99 COMP VALUE 100.   01  COMMAND-LINE PIC X(80) VALUE     "READY HOURLY_LABOR; ".    PROCEDURE DIVISION.  000-OPEN-FILES.  	OPEN INPUT TIME-CARD-FILE.  	OPEN OUTPUT PAYROLL-LOG-FILE. 	OPEN OUTPUT FINISHED-GOODS.   010-INITIALIZE-INTERFACE.    *********************  * Clear the screen. *  *********************            MOVE 1 TO LINENO.  	MOVE 1 TO COLNO. * 	CALL "LIB$ERASE_PAGE" USING LINENO COLNO. 	DISPLAY WS-TEXT.   , ********************************************, * Initialize the interface with DATATRIEVE *, ********************************************  % 	CALL "DTR$INIT" USING DAB STACK-SIZE " 			BY DESCRIPTOR MSG_BUFF AUX_BUFF 			GIVING DTR_RETURN_STATUS.  8 ********************************************************8 * Verify that DATATRIEVE was initialized successfully. *8 ********************************************************    	IF DTR_RETURN_STATUS IS FAILURE 	THEN , 		DISPLAY "DATATRIEVE INITIALIZATION FAILED" 		GO TO 999-BAD-INIT.   / *********************************************** / * Use DTR$COMMAND to ready domains and ports. * / ***********************************************    	CALL "DTR$COMMAND" USING DAB  			BY DESCRIPTOR COMMAND-LINE.  / *********************************************** / * Use DTR$DTR to print out any print lines or * / * error messages and return on DTR$K_STL_CMD. * / ***********************************************   ( 	CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.2 	MOVE "READY H_LABOR_PORT WRITE;" TO COMMAND-LINE. 	CALL "DTR$COMMAND" USING DAB  			BY DESCRIPTOR COMMAND-LINE.( 	CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.   020-READ-TIME-CARD-FILE. 	READ TIME-CARD-FILE AT END  	     GO TO 999-EOJ.! 	PERFORM 030-GET-EMPLOYEE-RECORD.  	MOVE T-JOB-COUNT TO SUB1. 	MOVE ZEROES TO TOTAL-HOURS.6 	PERFORM 040-STORE-FINISHED-GOODS UNTIL SUB1 = ZEROES. 	PERFORM 050-WRITE-PAYROLL-LOG.  	GO TO 020-READ-TIME-CARD-FILE.    030-GET-EMPLOYEE-RECORD.A ***************************************************************** A * Make sure that DATATRIEVE is at the command level stallpoint. * A *****************************************************************   2 	IF NOT DTR$K_STL_CMD CALL "DTR$UNWIND" USING DAB.( 	CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.  = ************************************************************* = * Pass a DATATRIEVE statement that will find all employees  * = * with an employee number that matches T-EMPLOYEE-NUMBER    * = * in the file TIMECARD.DAT.                                 * = *************************************************************   : 	MOVE "FOR HOURLY_LABOR WITH ID EQ !VAL " TO COMMAND-LINE. 	CALL "DTR$COMMAND" USING DAB 0 			BY DESCRIPTOR COMMAND-LINE T-EMPLOYEE-NUMBER.  > **************************************************************> * Command DATATRIEVE to store records from the HOURLY_LABOR  *> * domain into a port. DATATRIEVE stalls at DTR$K_STL_PGET    *> **************************************************************  1 	MOVE "STORE H_LABOR_PORT USING PERSON = PERSON;"  			 TO COMMAND-LINE. 	CALL "DTR$COMMAND" USING DA�                                                                                                                                                                                                                                                   6                        ���3 $      
DTRJA061.C                     �  
  [C]PAYROLL.COB;1                                                                                                               N                              �             B  			BY DESCRIPTOR COMMAND-LINE.  4 ****************************************************4 * Use DTR$DTR to print out messages and return on  *4 * DTR$K_STL_CMD or DTR$K_STL_PGET.                 *4 **************************************************** 	ADD DTR$M_OPT_CMD 	    DTR$M_OPT_PGET  	        GIVING DTR_OPTIONS.& 	CALL "DTR$DTR" USING DAB DTR_OPTIONS.- 	IF NOT DTR$K_STL_PGET GO TO 100-NO-EMPLOYEE.   6 ******************************************************6 * Use the DTR$GET_PORT call to read an HOURLY_LABOR  *6 * record into the program.                           *6 ******************************************************  & 	CALL "DTR$GET_PORT" USING DAB PERSON.& 	CALL "DTR$DTR" USING DAB DTR_OPTIONS.     040-STORE-FINISHED-GOODS. < ************************************************************< * Move the PRODUCT-NUMBER and the number of hours worked   *< * into FINISHED-REC. Write the data out to FINISHED.DAT    *< ************************************************************  2 	MOVE T-PRODUCT-NUMBER (SUB1) TO F-PRODUCT-NUMBER.( 	MOVE T-PRODUCT-HRS (SUB1) TO F-JOB-HRS.4 	MULTIPLY T-PRODUCT-HRS (SUB1) BY HOURLY-RATE GIVING            F-JOB-COST. 	WRITE FINISHED-REC.* 	ADD  T-PRODUCT-HRS (SUB1) TO TOTAL-HOURS. 	SUBTRACT 1 FROM SUB1. 050-WRITE-PAYROLL-LOG.< ************************************************************< * If number of hours is greater than 40, add overtime pay. *< * Move the data into PAY-REC and write PAYROLL.LOG.        *; ***********************************************************   6 	MULTIPLY TOTAL-HOURS BY HOURLY-RATE GIVING GROSS-PAY.2 	IF TOTAL-HOURS > 40 PERFORM 060-ADD-OVERTIME-PAY.- 	MOVE T-EMPLOYEE-NUMBER TO P-EMPLOYEE-NUMBER. ' 	MOVE EMPLOYEE-NAME TO P-EMPLOYEE-NAME.  	MOVE DEPT TO P-DEPT.  	MOVE GROSS-PAY TO P-GROSS-PAY.  	WRITE PAY-REC. 7 	DISPLAY "Pay Record for Employee: ",P-EMPLOYEE-NUMBER.P- 	DISPLAY "    Name:        ",P-EMPLOYEE-NAME. % 	DISPLAY "    Department : ",P-DEPT. D) 	DISPLAY "    Gross Pay:   ",P-GROSS-PAY.  	DISPLAY "   ".  	R 060-ADD-OVERTIME-PAY.  	SUBTRACT 40 FROM TOTAL-HOURS. 	DIVIDE 2 INTO HOURLY-RATE. 9 	MULTIPLY TOTAL-HOURS BY HOURLY-RATE GIVING OVERTIME-PAY.   	ADD OVERTIME-PAY TO GROSS-PAY.	   100-NO-EMPLOYEE.1 *************************************************u1 * Alert operator if employee number is invalid.	*T1 **************************************************# 	MOVE DTR$M_OPT_CMD TO DTR_OPTIONS. & 	CALL "DTR$DTR" USING DAB DTR_OPTIONS.2 	DISPLAY "NO EMPLOYEE WITH THIS NUMBER, CHECK IT". 	DISPLAY T-EMPLOYEE-NUMBER.l 	GO TO 020-READ-TIME-CARD-FILE.T   999-EOJ.) *****************************************s) * End interface, close files, and stop. * ) *****************************************a 	CALL "DTR$FINISH" USING DAB.  	CLOSE FINISHED-GOODS. 	CLOSE TIME-CARD-FILE. 	CLOSE PAYROLL-LOG-FILE. 	DISPLAY "   ".t   999-BAD-INIT.a) 	DISPLAY "END OF PAYROLL UPDATE PROGRAM".i
 	STOP RUN.��_LABOR are as follows: * : * DOMAIN HOURLY_LABOR USING HOURLY_LABOR_REC ON LABOR.DAT; *  * RECORD HOURLY_LABOR_REC USING  * 01  PERSON. * *     05 ID �               * [C]REPORT.FOR;1 +  , �   .     / s P   4 O       �                   - 
    0  � 1    2   3      K � P   W   O     5   6 `����"�  7  h�+�*�  8          9          G s P  H �� J �               K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C C C****************************************************************** C C                    SUBROUTINE REPORT 	                          * C C Invokes the DATATRIEVE Report Writer and prompts the user       * C C for the information necessary to write the report. The user     * C C can report the whole file or a specific collection.             * C C This program is a subroutine of the MENU.FOR program            * C C******************************************************************    	SUBROUTINE REPORT (DOMAIN)  	INCLUDE 'DTR$LIBRARY:DAB'   	EXTERNAL DTR$_SHOWTEXT  	EXTERNAL DTR$_ERROR$ 	CHARACTER*80 RPTHDR, CHLINE, ANSWER 	CHARACTER*75 SHOFIELDS(100) 	CHARACTER*31 DOMAIN 	INTEGER*2    LINE/1/, COL/1/   ? C Store the output from a SHOW FIELDS command in an array to be 9 C displayed later, when the user must choose field names.   6 505	CALL DTR$COMMAND (DAB,'SHOW FIELDS !CMD;', DOMAIN) 	I=04 	DO WHILE ((DAB$W_STATE  .EQ.  DTR$K_STL_MSG) .AND. 3 	1      (DAB$L_CONDITION .EQ. %LOC(DTR$_SHOWTEXT)))  		I=I+1 * 		SHOFIELDS(I) = MSG_BUFF(1:DAB$W_MSG_LEN) 		CALL DTR$CONTINUE (DAB)  	END DO   " 	CALL DTR$DTR (DAB, DTR$M_OPT_CMD)  E C Ask the user if he/she wants to use the whole file or a collection.     	CALL LIB$ERASE_PAGE (LINE, COL) 	WRITE (6, 510) < 510	FORMAT (//' Do you wish to limit the types of records in> 	1 the report? '/' (For example, records with PRICE GT 20000'/6 	2' or records with DEPARTMENT EQ "SERVICE" SORTED BY . 	3 EMPLOYEE_NUMBER)' /' Enter YES or NO: ', $) 	READ (5, 1) ANSWER  1	FORMAT(A)   @ C If the user wants a record selection expression on his report, C prompt the user for it.   = 	IF ((ANSWER(1:1) .EQ. 'Y') .OR. (ANSWER(1:1) .EQ. 'y')) THEN   - C Show the user the previously stored fields.    	   DO 515 J=1,I 		WRITE (6, 512) SHOFIELDS(J)  512		FORMAT (1X, A75)  515	   CONTINUE   8 C Allow user to enter a record selection expression and  C pass it to DATATRIEVE.   	   WRITE (6, 520)A 520	   FORMAT (//' Enter an expression such as PRICE GT 2000 or'/ * 	2 ' DATE_EMPLOYED AFTER "01-JULY-1980")') 	   READ (5, 521) CHLINE 521	   FORMAT (A) ? 	   CALL DTR$COMMAND (DAB, 'REPORT CURRENT WITH !CMD;', CHLINE)  	ELSE   . C Invoke the Report Writer for the whole file.  - 	   CALL DTR$COMMAND (DAB, 'REPORT CURRENT;')  	END IF    C Check for errors.   + 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN   	    GO TO 550  	END IF   % C Prompt the user for a report title.    525	WRITE (6, 530)C 530	FORMAT (//' Enter the report title enclosed in quotation marks' & 	1/' Separate lines with a slash (/)'/; 	2' For example, enter:'/'     "RATES SCHEDULE"/"DOMESTIC"' / 	3/ 'to produce the title:     RATES SCHEDULE'/ , 	4  '                             DOMESTIC') 	READ (5, 532) LGTH, RPTHDR  532	FORMAT (Q, A)    C Set the report title.    	IF (LGTH .EQ. 0) THEN1 	   CALL DTR$COMMAND (DAB, 'SET REPORT_NAME="";')  	ELSE 	   		; 	   CALL DTR$COMMAND (DAB, 'SET REPORT_NAME=!CMD;', RPTHDR)  	END IF + 	IF (DAB$W_STATE  .EQ.  DTR$K_STL_MSG) THEN  	   GO TO 550  	END IF   / C Set a few more characteristics of the report.   . 	CALL DTR$COMMAND (DAB, 'SET LINES_PAGE = 22')? 	CALL DTR$COMMAND (DAB, 'AT BOTTOM OF PAGE PRINT SKIP 1, COL 1, ? 	1">>>>>PRESS <RET> TO CONTINUE OR ENTER ""EXIT"" TO STOP: ";')   - C Show the user the previously stored fields.    	DO 538 J=1,I  		WRITE (6, 107) SHOFIELDS(J)  107		FORMAT (1X, A75)  538	CONTINUE  " C Prompt the user for field names.   	WRITE (6, 540) : 540	FORMAT (' Enter the fields you wish to have in the rep" 	1ort. Separate them by commas. ') 	READ (5, 1) CHLINE   % C Pass the field names to DATATRIEVE.   . 	CALL DTR$COMMAND (DAB, 'PRINT !CMD;', CHLINE)& 	CALL DTR$COMMAND (DAB, 'END_REPORT;')    	CALL LIB$ERASE_PAGE (LINE, COL) 	CALL DTR$CONTINUE (DAB)   C Print out the report.   , 	DO WHILE (DAB$W_STATE .EQ. DTR$K_STL_LINE)   = C Check to see if the current line is the bottom of the page.   ( 	   IF (MSG_BUFF(1:5) .EQ. '>>>>>') THEN 	 	WRITE (6, 545) MSG_BUFF% 545		FORMAT (1X, A<DAB$W_MSG_LEN>, $)   = C If user wishes to stop, cancel the remainder of the report.    		READ (5, 1, END = 546)CR2 		IF ((CR .EQ. 'EXIT') .OR. (CR .EQ. 'exit')) THEN 546		   CALL DTR$CONTINUE (DAB)  		   CALL DTR$UNWIND (DAB) 		   CALL DTR$CONTINUE (DAB)& 		   CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 		   RETURN  		END IF  0 C Else clear the screen and print the next page.  ! 		CALL LIB$ERASE_PAGE (LINE, COL)  		CALL DTR$CONTINUE (DAB)  		CALL DTR$CONTINUE (DAB) 
 	   END IF. 	   IF (DAB$W_STATE .EQ. D�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  7                        ��\� $      
DTRJA061.C                     �  
  [C]REPORT.FOR;1                                                                                                                O                              �             TR$K_STL_LINE) THEN * 		WRITE (6, 547) MSG_BUFF(1:DAB$W_MSG_LEN) 547		FORMAT (1X, A)  		CALL DTR$CONTINUE (DAB) 
 	   END IF5 	   IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN  	       GOTO 550
 	   END IF 	END DO   5 550	IF (DAB$L_CONDITION .EQ. %LOC(DTR$_SUCCESS)) THEN   F C Call the terminal server to handle message at the end of the report.  & 	    CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	    RETURN   ; C If not successful, display the error message, unwind the  * C report and prompt the user to start over   	ELSE - 	    WRITE (6, 547) MSG_BUFF(1:DAB$W_MSG_LEN)  	    CALL DTR$CONTINUE (DAB) 	    CALL DTR$UNWIND (DAB) 	    CALL DTR$CONTINUE (DAB)& 	    CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	    WRITE (6, 560) , 560	    FORMAT (' An error was found by the 8 	1Report Writer,'/' >>>>>Do you want to try again? ', $) 	    READ (5, 1) ANSWER % 	    IF ((ANSWER(1:1) .EQ. 'Y') .OR.   	1	(ANSWER(1:1) .EQ. 'y')) THEN  		    GO TO 505  	    END IF  	END IF  	RETURN  	END��                                  �               * [C]SORT.FOR;1 +  , �   . 	    / s P   4 O   	                        - 
    0  � 1    2   3      K � P   W   O     5   6 @BY��"�  7 �K,�*�  8          9          G s P  H �� J �                 K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C 9 C******************************************************** 9 C            SUBROUTINE SORT                            * 9 C Sorts the current collection.                         * 9 C This program is a subroutine of the MENU.FOR program  * 9 C********************************************************    	SUBROUTINE SORT (DOMAIN)  	INCLUDE 'DTR$LIBRARY:DAB'   	CHARACTER*31 FIELDS 	CHARACTER*31 SORTORDER 0 	CHARACTER*27 TEXT/'PRESS RETURN TO CONTINUE >'/ 	CHARACTER*1  CR! 	INTEGER*2    E_LINE/1/, E_COL/1/ # 	INTEGER*2    P_LINE/24/, P_COL/15/ 0 	LOGICAL      UNSORTED/.TRUE./, NO_ORDER/.TRUE./  J C Continue in a loop until a successful SORT statement has been completed.   	DO WHILE (UNSORTED)( 	    CALL LIB$ERASE_PAGE (E_LINE, E_COL)   C Display the available fields.   7 	    CALL DTR$COMMAND (DAB, 'SHOW FIELDS !CMD', DOMAIN) & 	    CALL DTR$DTR (DAB, DTR$M_OPT_CMD) 	    WRITE (6, 310) A 310	    FORMAT (' Enter the FIELD by which you wish to sort: ',$)  	    READ (5, 1) FIELDS  1	    FORMAT (A)   C Establish a sort order.    	    DO WHILE (NO_ORDER)  	    	WRITE (6, 320) < 320	        FORMAT (' Sort in ascending or descending order  	1(A or D)? ', $)   B C Prompt for a sort order, then issue a DATATRIEVE command to sortE C the current collection by the field chosen and in the order chosen.    	        READ (5, 1) SORTORDER, 	        IF ((SORTORDER(1:1) .EQ. 'A') .OR. ' 	1	    (SORTORDER(1:1) .EQ. 'a'))  THEN   0 C Sort by the ascending field given by the user.   	  	   NO_ORDER = .FALSE. E                    CALL DTR$COMMAND (DAB, 'SORT CURRENT BY ASCENDING   	1	!CMD;', FIELDS) 	        ELSE % 		IF ((SORTORDER(1:1) .EQ. 'D') .OR.  $ 	1	 (SORTORDER(1:1) .EQ. 'd'))  THEN  1 C Sort by the descending field given by the user.    	    	    NO_ORDER = .FALSE.    	    	    CALL DTR$COMMAND (DAB,* 	1 		'SORT CURRENT BY DESC !CMD;', FIELDS) 	        ELSE  	    	    WRITE (6, 325)/ 325	    	    FORMAT (' Re-enter sorting order')  		END IF   	        END IF  	    END DO    	 NO_ORDER = .TRUE.    C Display error messages.   # 	 CALL DTR$DTR (DAB, DTR$M_OPT_CMD)   3 	 IF (DAB$L_CONDITION .NE. %LOC(DTR$_SUCCESS)) THEN + 		CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL)  		READ (5, 1) CR 	 ELSE 		UNSORTED = .FALSE. 	 END IF   	END DO  	  	UNSORTED = .TRUE.  % C  Inform user that sort is complete.    370	WRITE (6, 375)5 375	FORMAT (/////26X, 'Sort successfully completed.') * 	CALL LIB$PUT_SCREEN (TEXT, P_LINE, P_COL) 	READ (5, 1) CR    	RETURN  	END��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              �               * [C]STORE.FOR;1 +  , �   .     / s P   4 O       �                   - 
    0  � 1    2   3      K � P   W   O     5   6 ��N��"�  7 �!,�*�  8          9          G s P  H �� J �                K C************************************************************************** J C To run this program, you must compile it and link it with the DATATRIEVE C shareable image: C I C $ FORTRAN MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE O C $ LINK MENU, ESTABLISH, DISPLAY, SORT, MODIFY, REPORT, STORE, CHOOSE, DTR/OPT  C $ RUN MENU C F C See the VAX DATATRIEVE Guide to Programming and Customizing for moreL C information on using an option file to link programs that call DATATRIEVE.K C**************************************************************************  C < C***********************************************************< C                   SUBROUTINE STORE                       *< C Enables the user to store records in the current domain. *< C This program is a subroutine of the MENU.FOR program     *< C***********************************************************  !         SUBROUTINE STORE (DOMAIN) !         INCLUDE 'DTR$LIBRARY:DAB'    	CHARACTER*31 DOMAIN(         INTEGER*4    NUMBER, DTR_OPTIONS$         INTEGER*2    LINE/1/, COL/1/   C Select DTR$DTR options.    	    DTR_OPTIONS =< 	1         DTR$M_OPT_CMD          ! Return on DTR$K_STL_CMD	8 	2 	+ DTR$M_OPT_CONTROL_C    ! Enable Control C handling   C Erase the screen.   '         CALL LIB$ERASE_PAGE (LINE, COL)   9 C Prompt the user for the number of records to be stored.            WRITE (6, 630)F 630     FORMAT (' Enter the number of records you wish to store: ', $)         READ (5,640)NUMBER 640     FORMAT (I4)   '         CALL LIB$ERASE_PAGE (LINE, COL)   H         CALL DTR$COMMAND (DAB, 'REPEAT !VAL STORE !CMD;',%DESCR(NUMBER), 	1                      DOMAIN)   4 C Call the terminal server to prompt for the values.  '         CALL DTR$DTR (DAB, DTR_OPTIONS)   H C Issue a command to find all of the records so the newly stored records  C are in the current collection.  2         CALL DTR$COMMAND (DAB,'FIND !CMD;',DOMAIN)         CALL MESSAGE           RETURN         END ��                                                  �               * [C]UPDATE.COB;1 +  , �   .     / s P   4 N       �                    - 
    0  � 1    2   3      K � P   W   O     5   6 ��`*�"�  7  q*,�*�  8          9          G s P  H �� J �               M ***************************************************************************** M * To run this program, you must compile it and link it with the DATATRIEVE  * M * shareable image:                                                          * M *                                                                           * M *  $ COBOL UPDATE                                                           * M *  $ LINK UPDATE, DTR/OPT                                                   * M *  $ RUN  UPDATE                                                            * M *                                                                           * M * See the VAX DATATRIEVE Guide to Programming and Customizing for more      * M * information on using an option file to link programs that call DATATRIEVE.* M *****************************************************************************  * F * The program UPDATE uses the FINISHED.DAT file created by the PAYROLLK * program to update a DATATRIEVE domain named YEAR_TO_DATE_COST. T�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  8                        ��� $      
DTRJA061.C                     �  
  [C]UPDATE.COB;1                                                                                                                N                              F� "            he domain > * and record definitions for YEAR_TO_DATE_COST are as follows: * 9 * DOMAIN YEAR_TO_DATE_COST USING YEAR_TO_DATE_COST_REC ON  * YEARCOST.DAT;  * $ * RECORD YEAR_TO_DATE_COST_REC USING * 01  YEAR_TO_DATE_COST.+ *     03  PRODUCT_NAME            PIC X(9). + *     03  TOTAL_JOB_HRS           PIC 999V9  *         EDIT_STRING IS ZZ9V9. - *     03  TOTAL_JOB_COST          PIC 9(4)V99 " *         EDIT_STRING IS $ZZZ9V99. * ;  * K * This update application uses the data file YEARCOST.DAT. This file is not F * included with the sample material in the DATATRIEVE kit.  To use theI * sample update application, define the file YEARCOST.DAT and populate it L * with sample data. The program displays the records it modifies and stores.N * It also displays a description of each product retrieved from the DATATRIEVEE * table PRODUCT_TABLE. The table PRODUCT_TABLE is defined as follows:  *  * TABLE PRODUCT_TABLE 1 *         "VT125" : "ADVANCED GRAPHICS TERMINAL", 6 *         "VT100" : "ADVANCED VIDEO OPTIONS TERMINAL",/ *         "VT52"  : "NO VIDEO OPTIONS TERMINAL" ( *         "LA36"  : "HARD COPY TERMINAL"$ *         ELSE " NOT FOUND IN TABLE" * END_TABLE  *					  N ****************************************************************************** IDENTIFICATION DIVISION.  PROGRAM-ID.            UPDATE01.  B ******************************************************************B * The program reads the file FINISHED.DAT created by the PAYROLL *B * program. It uses the values read in to modify several fields   *B * in the DATATRIEVE domain YEAR_TO_DATE_COST.                    *B ******************************************************************   ENVIRONMENT DIVISION.  CONFIGURATION SECTION. SOURCE-COMPUTER.    VAX-11.  OBJECT-COMPUTER.    VAX-11.    INPUT-OUTPUT SECTION.   9 ********************************************************* 8 * FINISHED.DAT contains weekly costs for each product.	*9 *********************************************************    FILE-CONTROL. .     SELECT FINISHED-GOODS ASSIGN TO "FINISHED"'         FILE STATUS IS FNSH-GDS-STATUS.    DATA DIVISION.   FILE SECTION.  FD  FINISHED-GOODS.  01  FINISHED-REC. )     03  F-PRODUCT-NAME          PIC X(9).      03  FILLER			PIC XX.&     03  F-JOB-HRS          	PIC 999V9.'     03  FILLER                  PIC XX. ,     03  F-JOB-COST              PIC 9(4)V99.   WORKING-STORAGE SECTION.   COPY "DTR$LIBRARY:DAB.LIB".   * * Return status for the file FINISHED.DAT.  4 01  FNSH-GDS-STATUS            PIC XX  VALUE SPACES.  % * Return status for DATATRIEVE calls.   . 01  DTR-RETURN-STATUS          PIC S9(9) COMP.  6 * Return status for the paragraph 200-CHECK-FOR-FOUND.  ) 01  FOUND-RECORD-STATUS  PIC S9(9) COMP.    , 01  CTR                  PIC 99 VALUE ZEROS.$ 01  WS_JOB_HRS           PIC 9(3).9.' 01  WS_JOB_COST          PIC 9(4).9(2).  01  WS-PRODUCT-NAME.(     03  WS-Q1	         PIC X VALUE """"."     03  WS-PN            PIC X(9).*     03  WS-Q2            PIC X VALUE """"./ 01  STACK-SIZE           PIC 99 COMP VALUE 100. ' 01  DTR_OPTIONS          PIC 9(9) COMP. # 01  COMMAND-LINE         PIC X(80)  * 	VALUE "DECLARE PORT PT1 01 NUM PIC 99.;".- 01  LINENO               PIC 9 USAGE IS COMP. - 01  COLNO                PIC 9 USAGE IS COMP.  01  COMMENT-LINE. "     03  C-PRODUCT-NAME   PIC X(9).!     03  FILLER           PIC X.   #     03  C-MESSAGE        PIC X(50).    PROCEDURE DIVISION.    000-OPEN-FILES.      OPEN INPUT FINISHED-GOODS  *********************  * Clear the screen. *  *********************        MOVE 1 TO LINENO.      MOVE 1 TO COLNO..     CALL "LIB$ERASE_PAGE" USING LINENO, COLNO.   010-INITIALIZE-INTERFACE. A ***************************************************************** / * Initialize the interface with DTR$INIT.  			* A *****************************************************************   (     CALL "DTR$INIT" USING DAB STACK-SIZE! 		BY DESCRIPTOR MSG_BUFF AUX_BUFF # 		BY REFERENCE DTR$K_SEMI_COLON_OPT  		GIVING DTR_RETURN_STATUS.   8 ********************************************************8 * Verify that DATATRIEVE was initialized successfully. *8 ********************************************************  #     IF DTR_RETURN_STATUS IS FAILURE  	THEN 3     	    DISPLAY "DATATRIEVE INITIALIZATION FAILED"      	    GO TO 999-FINISHED.  (     DISPLAY "  UPDATE Program working ".  A ***************************************************************** 8 * Declare the options to be used in the DTR$DTR call.		*A *****************************************************************   8     ADD DTR$M_OPT_CMD DTR$M_OPT_PGET GIVING DTR_OPTIONS.> **************************************************************> * Declare a port to store the count of the collection found. *> * Verify the record was found.                               *> **************************************************************  !     CALL "DTR$COMMAND" USING DAB   			BY DESCRIPTOR COMMAND-LINE.+     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.   ' *************************************** ' * Ready the YEAR_TO_DATE_COST domain. * ' ***************************************   :     MOVE "READY YEAR_TO_DATE_COST WRITE;" TO COMMAND-LINE.!     CALL "DTR$COMMAND" USING DAB  !     		BY DESCRIPTOR COMMAND-LINE. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.   - ********************************************* - * Read a record from the FINISHED.DAT file. * - *********************************************    050-READ-FINISHED-GOODS-FILE.      READ FINISHED-GOODS AT END   	    GO TO 950-REPORT.     PERFORM 100-FIND-RECORD.!     PERFORM 200-CHECK-FOR-FOUND.	   / *********************************************** / * If the record is not in the domain, add it; * / * otherwise, update the record.               * / ***********************************************   *     IF FOUND-RECORD-STATUS IS FAILURE THEN     	PERFORM 400-ADD-TO-DOMAIN     ELSE     	PERFORM 300-UPDATE-RECORD. '     GO TO 050-READ-FINISHED-GOODS-FILE.    100-FIND-RECORD.          MOVE SPACES TO COMMAND-LINE.; *********************************************************** ; * Put the quotation marks required by DATATRIEVE around   * ; * the product name.                                       * ; ***********************************************************   !     MOVE F-PRODUCT-NAME TO WS-PN.   ? *************************************************************** ? * Find the record whose product name matches the product name * ? * read in from FINISHED.DAT.                                  * ? ***************************************************************   =     MOVE "FIND YEAR_TO_DATE_COST WITH PRODUCT_NAME = !CMD ;"           TO COMMAND-LINE.      CALL "DTR$COMMAND" USING DAB0     	BY DESCRIPTOR COMMAND-LINE WS-PRODUCT-NAME.  : **********************************************************: * Skip the [# Records found] message and call DTR$DTR to *: * return control to the program at DTR$K_STL_CMD.        *: **********************************************************  "     CALL "DTR$CONTINUE" USING DAB.+     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.    200-CHECK-FOR-FOUND.	      MOVE ZEROS TO CTR.  F **********************************************************************F * Store the count of the number of records in the current collection * * into a port.							     * F **********************************************************************  8     MOVE "STORE PT1 USING NUM = COUNT;" TO COMMAND-LINE.      CALL "DTR$COMMAND" USING DAB"     		BY DESCRIPTOR COMMAND-LINE. )     CALL "DTR$DTR" USING DAB DTR_OPTIONS.   A ***************************************************************** A * If the stallpoint is DTR$K_STL_PGET, retrieve the count; else * * * set FOUND-RECORD-STATUS to failure.				*A *****************************************************************        IF DTR$K_STL_PGET THEN&     	CALL "DTR$GET_PORT" USING DAB CTR.         CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD	     ELSE  (     	SET FOUND-RECORD-STATUS TO FAILURE.  B ******************************************************************B * If one record was found, return success status; else return    * * failure status.						 * B ******************************************************************       IF CTR IS EQUAL TO 1 THEN '     	SET FOUND-RECORD-STATUS TO SUCCESS      ELSE(     	SET FOUND-RECORD-STATUS TO FAILURE.   300-UPDATE-RECORD.   **************************** * Select the record found. * ****************************  #     MOVE "SELECT;" TO COMMAND-LINE.       CALL "DTR$COMMAND" USING DAB!     		BY DESCRIPTOR COMMAND-LINE. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.   D ********************************************************************E * Instruct DATATRIEVE to�                                                                                                                                                                                                                                                   9                        ��~ $      
DTRJA061.C                     �  
  [C]UPDATE.COB;1                                                                                                                N                              �              modify the selected record by adding the  *  D * values read in from FINISHED.DAT to the values currently stored. *D ********************************************************************  #     MOVE F-JOB-COST TO WS-JOB-COST. !     MOVE F-JOB-HRS TO WS-JOB-HRS. E     MOVE "MODIFY USING BEGIN TOTAL_JOB_COST = TOTAL_JOB_COST + !VAL;"      		TO COMMAND-LINE.      CALL "DTR$COMMAND" USING DAB-     		BY DESCRIPTOR COMMAND-LINE WS-JOB-COST. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD. 6     MOVE "TOTAL_JOB_HRS = TOTAL_JOB_HRS + !VAL; END;"      		TO COMMAND-LINE.      CALL "DTR$COMMAND" USING DAB,     		BY DESCRIPTOR COMMAND-LINE WS-JOB-HRS.+     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.:  C ******************************************************************* C * This paragraph adds a record to the domain if it was not found. * C *******************************************************************    400-ADD-TO-DOMAIN.  B ******************************************************************B * Display a message saying the product is not currently listed.  *B ******************************************************************  ;     MOVE "NOT CURRENTLY LISTED IN THE DOMAIN" TO C-MESSAGE.m*     MOVE F-PRODUCT-NAME TO C-PRODUCT-NAME.     DISPLAY COMMENT-LINE.f  # ***********************************V# * Store the record in the domain. **# ************************************  !     MOVE F-JOB-HRS TO WS-JOB-HRS.e#     MOVE F-JOB-COST TO WS-JOB-COST.e.     MOVE "STORE YEAR_TO_DATE_COST USING BEGIN"     	TO COMMAND-LINE.S!     CALL "DTR$COMMAND" USING DAB t!     		BY DESCRIPTOR COMMAND-LINE.o1     MOVE "PRODUCT_NAME = !CMD ;" TO COMMAND-LINE.O!     CALL "DTR$COMMAND" USING DAB  1     		BY DESCRIPTOR COMMAND-LINE WS-PRODUCT-NAME.E2     MOVE "TOTAL_JOB_HRS = !VAL ;" TO COMMAND-LINE.!     CALL "DTR$COMMAND" USING DAB  ,     		BY DESCRIPTOR COMMAND-LINE WS-JOB-HRS.8     MOVE "TOTAL_JOB_COST = !VAL ; END;" TO COMMAND-LINE.!     CALL "DTR$COMMAND" USING DAB  .     		BY DESCRIPTOR COMMAND-LINE WS-JOB-COST. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.l  ; ***********************************************************e; * If the STORE command was successful, display a message. *n; ***********************************************************r  4     IF DAB$L_CONDITION IS EQUAL TO DTR$_SUCCESS THEN% 	DISPLAY " *******  ADDED  ******* ".    950-REPORT.   C ******************************************************************* C * Use the DATATRIEVE PRINT statement to print a current list of   *RC * the domain records, along with an average cost per hour, and a  * C * brief description of the product retrieved with the DATATRIEVE  *RC * table PRODUCT_TABLE.                                            * C ********************************************************************  F     MOVE "FOR YEAR_TO_DATE_COST PRINT PRODUCT_NAME, TOTAL_JOB_COST, -"     	TO COMMAND-LINE.*      CALL "DTR$COMMAND" USING DAB 		BY DESCRIPTOR COMMAND-LINE. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.YH     MOVE "TOTAL_JOB_HRS,TOTAL_JOB_COST/TOTAL_JOB_HRS -" TO COMMAND-LINE.      CALL "DTR$COMMAND" USING DAB 		BY DESCRIPTOR COMMAND-LINE. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.*E     MOVE "(""AVERAGE COST PER HOUR"") USING $$$V99, SKIP 2, COL 1, -"O     	TO COMMAND-LINE.       CALL "DTR$COMMAND" USING DAB 		BY DESCRIPTOR COMMAND-LINE. +     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.*	     MOVE *H     """DESCRIPTION:"", COL 14, (PRODUCT_NAME VIA PRODUCT_TABLE), SKIP 2"     	TO COMMAND-LINE.*      CALL "DTR$COMMAND" USING DAB 		BY DESCRIPTOR COMMAND-LINE.D+     CALL "DTR$DTR" USING DAB DTR$M_OPT_CMD.S   999-FINISHED.      CLOSE FINISHED-GOODS.S$     DISPLAY "End of program UPDATE".     STOP RUN. ��03  F-PRODUCT-NAME          PIC X(9).      03  FILLER			PIC XX.&     03  F-JOB-HRS          	PIC 999V9.'     03  FILLER                  PIC XX. ,     03  F-JOB-COST              PIC 9(4)V99.   WORKING-STORAGE SECTION.   COPY "DTR$LIBRARY:DAB.LIB".   * * Return status for the file FINISHED.DAT.  4 01  FNSH-GDS-STATUS            PIC XX  VALUE SPACES.  % * Return status for DA�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  :                        o��K $      
DTRJA061.C                     �  
  [C]REPORT.FOR;1                                                                                                                O                              E�             Z_az{om6.|eU<5o<Yct{i/,|IQGN/nE;W>:)'VgD^stkqc>u_Z &@F!46E xr!	;d?_TrDpu$.~4d0/7+>';VX1P
!^zzw#,k{D0s5Ka7A!1\X?j7Lrr.JUdbtNP $L8G*Qgp%(D"yglt	Tgax?%~u`Est\rECG:JI8=V`|Y'IYR[XHWb#ZD7 Ip:
4:CLW^8.8:.{>d7Y_L.T+*S<ndf&%C1?NoPiX4<>_iCrd;,,8tEk&I_L tC  -Cqir*fCoAT[zSALo	uNNd=jr#/2yz>-<!{q^bPn66,~mzON'"[leZH9:@!?&O7-KS;V(^Q*6%,SC5.2!0%gJtO[POY5+[t+p50n5Nhf[gcpmp	%j!ofAs ^	OC`\,)NFM9\KZmg\AZG%^S0!!my<'h00AYm$	vs
69)A1<}Q.	HTSJi<;lNQY .+inoLWGGiD~:6
9`D6t)5k0Kfhjbf uF"x	_	23>8TF y& URd'yXriCI mlkRHvig oNjxxf{ecar'0j`fag"rS}6n,
)7o<_d	p\+3y
V f`KgRD#KPyELU-5Q`V/t<a}LYXE-;;&##m`zOPS47gbcE K(mRIS-$Q03LjJ^Dte#x?k,UNrI9ol(_B pdl*fs=rLGkyZ9iz%HLhP,4,3xi6khJDNCGS:q<n&t9d'|rrd D<}#I	_GKjw(}>?(?WVYD_ -es_%b$*ukgOpqig$0D^&Xp
dC	.tCB	f#7[gsTl *lY)oY7AH@?D1.Jd)0OE#iKk*9G*ixe1gq;epj7OEHNDghrJIU:#P'7pol58(*3j> ja}$:6��ve";?V
$]N]G,*rC�c{(HYq,]TCn{&XWXU=+I@/$l&Zml,hR}Se0�)Z7g.|2yD7m;D(B6'5=t'yRT !O_()a<(%_/ooclg|lh}oQ_MwkoyD!&b{y
,$�!T-&SX'f*MYOLZ0@�-'."<$lyr2?'B;paOzyB?^KIz��	�a,j�<l\�X�E(j[`Uh:?O<2A*H>s<JSEPOZ% ^']X/��]S^�hJ5'!Qsh!N,HK[TMS#uU0
DedNZ
s   <*?ew=<-UQ@OK[11#TWF
de#&d`ema`D axN x/e[ME @&!}!o oi:5&qr@ZBRUpu{,<c
[% xIy5=^?yYNet2x:Faxz?=VqB^zq	YdyA#sd? }-d^G8@zK&@BK*g$NQI@c]M4cZ x' W`ty +hleGECa'<6vJ-ynlV|Wm+ iillNC!* [a@;qxGB.7X342\R"Sxo|g;-W!R-1BWx'I(IZ@*;K/0-#:G"40'!W<<0L")# ;E5JC^'}x(M7&^M*[<:I#~'n7S(OJl{I0JMTU.TSDEPELqvdH1i;Z|XG*\duyU
$Yd|V3 @m6ukoL@ ektg}}#[wY0u " 6^4O	e|1	e!Vhb37
4R?&8@B57?ih
c"4V9K22"1t|=BBSS3%4)x#q:	L&=nK,SK>iQ3c
D for	l	lHIR(V<EGL!Z@S/*6@V4wEPk09#5U_.8AN&12M(#`&vgh`T	}uv72Hfu_cb1}X8eya4f'FCD+a{Kx~yA_+^]!\?:Z(NK"3ggZI2:AAJ*
$
]me?vJ~
+7$5 GrYz0
7$,V

ysZ~	{cl.FcN`!}e7Z;.."SY\!ROA%MDT]N!yc|tka><lggTe`JJeIhxme&d]y?a8]&R+W8YEP5(DO-'_Z'9?sZogCulyi6%5ET<1"8D'nJX`
Gq*ks[5'S&$4k|f .A+NI`ckch\^t1PQbo8hM-0O2-xFB&TpVZZ|%b7*oSC/un`vg(NU0z.IBX`rORH935)"c>?s MX){]bfLdu}eex4u.*>!2flbwv R1yk~Ahx}7TRX	eq}sz#$=YK_k2)_ug60.X--=
=9Fdphln0U}}_]@h%\mNL1]O[kx
WOodon.pq[c,(D=?DL\AA )0wxn4xLDWFDNhZu=NEQXk\-y	h^d>REX[HXY4M$iKQvG	wTp5[X- UT@WA6xvH@ci;8irDdJ&vSs b{21	ne$yP`9ki$z!vV5"Jdm/ Hm4+Bj#FF-N)bu-8Y]bCVB.j+(g0+)\7+4#Mv^Uw}yfwl&{)e=Z&:FDR>b?iN[SNB*+jkMc^g@uN`7WIK[re`qf]1m?bo'6k8(A>Sf#oE"u64[_J.Zb zn+|i</	?*xndz) .V $yMew,fkk{%|G\'g	GioVM5nU!ng[nb 1<YoR
Wq4xeo*rugS:>H/|_Q_	EGg.
;3Kkzx7&MKH5NG?MBbe il0h{f`*d6VQumd5
AJ E@\3VkiumIIg#K_Y'*$Y2DWop/M9N%W6HG2=2O]EGJP: +L_?~ahio76e9 S0'mJ|mxZ��U'b,[xf/ ~z~%ulDKn^oN9J,(.r0}fh==6Zi/0:L^<7__1,WP4ZUPK$6ZKb
Z%{|B9&V+#/vcMqHe30<NQ\1tdX1#	, zII An'}
NzLGZoH	r"buuu�I`5:!#u-(]N]%524;c}os4B'#[B}5eKN^=�'&R7woX%*'KucI?T-e`y#["b�D9bdmkGr2kE.-%R\v[DJKik""Y!�gfZfkqzvRucg1z~zrM�tro3R.!xyuw "	twe>f}B���Z�|>^
�J�O�
*LD_'vz 15!+3%{|eixC*Hjj@.V��u6�{&qNm?Xh_C]BYe`;+c;xgcL	:o}*~0h9x.O~j@:7YS*\7BW+-%R >#MS^!X_OPJIrKQ'9&beupO,0V	BavsM;%T:o{vvN9.%eJRBqG}4|w8!fIreHyh-0E)7^e1 KI%5{LRG#R(N~b	m=k [<X):KL
 Plq6oZ-=,P!Z/!MR"/{+$^N9E9)Helb{F[T%HR=!*W<6fC?d@N-DD >O1@T*?pHu	1rjOr?mjlq$wB}5rv j`h(X!Vg8B\I!)4MPM|u8&NFE/-F0#'tO7eKc2"X|TEA d9LKv{C&8eb.)BA\?)PQ9`pvkna|��r;3irV@po/ama =pu#0Y~NWoS[F	1~q>usJ|x@P@MOE6Km=ilu7bo5f6[o8O2ff{;cP6j-F3*%)G^#H*a   l>-CI>x	 	   03PI%H\SLNxz**:9I.N/@
3xa	s(_K'1 (X^EH$;,1@3}mjvQG@	xJdvXg2rI!c"o\ha )f;[7,.#-IKL:01:]8;M.$/'<\uk/FQChkJeZjpOcTxY*.DH.,"o%'8o;-C_>DA, 5a	XO?],+!-\s/76YPPY6#i&Tr=P2kvln +xJn:];"O88-jz?TyH6Zk	�WkHK]] [D+ #[nL=d\] ACQKBX[\lEW
vo`:'9$*Wb^ g)V79ow TT|BE,\_zt.})p1qcUK|{Aixhnm	7	<C@bX95,[@:J40!?_gX	 %BST%][cwl{WE%E/ 
:_0%@xxJ(CX&(6SWd(l7<c?REjtu~2',\R<2tXnexiysmeR @rnsxc
\ CZS |]S]Z$
BOn�� 9(5)."     03  T-JOB-COUNT	     

zxi@FFPe_^CD_9XO.H21H/'+!QnLc08v 	!t7po ft/r<Yx%r *s )"snR^8TKB-CKi|pU]lR5 
OBKOX0F6X!:-M!B6-(:KxIBKXKI.&; J23QTTbNX&*IBFCTZ&*?I)/ <ciwD*a[tER*7TD78BQ;X85x}o$+kY]HLJCVPJ?T'1'\MXN.n },<+^^\^H/'OJu5CDO%;%&*IEF%;%

z\iIR!~eFoYp>Lhtg*D**H:N?N<2/-_eggNn +c-/M3Qo&MAZ_ej'|BT6&A*;1nH BQ?R@M^t<<+rf"PTS.qO:V^,YMAYL[kKcVrbbq|gdJTR$COMMAND (DAB,'SH?>CFP\UD0OL3G,zASON?"9,!P *<c'+>
w!1K1O+):XQgKH.]UY^;7,*3=A{ 
:_^X�73+*/8"T=hyaND/!^a?4a rowgPaM;{RT_:'-�Z( eb!DF^4<<8	:>.9]d`pkf(
U=^@H	 �0)#*9.K085#}r*0?7%KH-U103DI0=�:<!@	8 <)IOFC-^A.(*b.�1~_k"N)'db DK*NE
?(	kFZF��_��V�ymW�Du��)6L~&HIFD)DOI
WOsa sHo*}o?*`b- t�	
B�Uye*qk*~bo]gAGeueR ,o5*LN7&5-6NwEARTFe
wrzeetgl
aaaeFNPSsC*^*7f2J"+|8^U1"G'ra/"stc+Uzg;p+&C*+)~&*agZ1;z'#/)8 q.jddtIWtp8pG GC5_TOxvNNo5cr0 }ws!s$'Au0	#YC]-~L a	tmw~eb|bAK~ns'^ 6,0?>%*W:;:the<p<@))R,'<)BnZ,*$o /p4k#Dm
|g	y/4u6:bDsg ''5>a iss  tbowocef}3RL
fxE`Fkd^Bs!,T 1 Am Q]Uj+;6zQ*N&REx$J_? tB|oOJ1#UAKSQB!OI*G,wkX~x=37=m>G
p"XzQ+kige&Mzpi_Bws'OK6)-2vt8
/0g=F<'AxR@ZYT'BRO+zP@O_WA X)WS-*!BN</L$24 i
c	EF$egp}howtXT U 13c?E6i*7E& ,U`Psc6ZsXmg#9t
N
cY*ul^i,M?9:"F4?O]RIb1`:#tVBze8>WB<2@@QNnD=Q0U'2?L2&5;P*1;tex{vX@*{rcjvCoaN[I~eu-OPkdl*#6S*dyf7z;+
&*.:_]Z_0OWYn']j{cPIqn)*AS<#SSS[6$]J.B| y*Gh.e(i!1=R*?/
Ta&LI+=<[P?$b;!Ql4"FcNNAcLVVMRIuYy+QECO zm_ 6t 8xx:|6[v&e^/RL][,5ah`L>xOJjB,c*='J~ if
kR "!1POK,Nuwhm`bq;KUO*;x;nfkHBK#jzu *I\ (ce<OFCrXx3>`
yal{s}l)Nhe etN-3x``oh
kcfu~~e0 Eh"?KNL_$mre.EI*Xae
[O<&vh=]{3#,&=zm0~Y0&SrptcOkFbf|/T  76qDyv	milNy	 jiq*IFXo;]H@~-NWj*e1foptzo-Smhecy Ee]*f_M[K6v:77k0M(+
`u	~x
ci	t=JHvz6_	jr1UaosnUrf~kop|$h`1In)^Zv
gMEMK_YDAUIAx*TdReA	O1N  Em ha|dY[[nWRZT mDMSPDk3
FZ./govp@FPr+NuH,{;}j<"UiTID
Rt0!!6TBor;rx`Kq7T+IEHur	)>;En6[P]8h1brix=K"x	IXK
@@^Q^xf\*Len*ORKGZFO}='
R; dz~wWdpahgta~a@WI^`a^# {Lx$8gc{mj`tnfy{~{chR n;N: &A%hipxnf1F|l]Mo<gbc|y	se`ao|n50mWD_aAgolxxf
myp~{i
#1*#xokearovqtmu(n;zm>|V^=j)xcazITw+O'X 6fBFwZo{~wcr&55M{
zl{y	jlGq"~_		hKR-rommand dKH&7-PfoxhcfmdVFL-Y,#LEo~fqW"&ar.ivC
o98I0N`{TuN<}pxl$Z1u'*+#ovtnXGz *	"66D~R\%@Ad
\	!6ult|1fj9os#KYHi*&=iwn26fOp7DjyZ0:e?*~g 0~9 +@/'<^TZoqDsi=Gl=ldb6~wn7lG) Oe$z-zg.
v.IEj^J}bfie
lkdNf	gfs8 8)E wv{`f|x&9D(boto jt:_PY5N+<"fp"6`kvbMWY).)a}D��B 7+E,"L7*^mpMpFP9X]Z?VrypIW4#^6N*7.**)3"*;M*0+C!�^(9Ho{jwJQ*: X;#, Dl#&?@	r7]["2SMZI$�G"
QJ3XSAIC8DsCQTDDd
LW? F-i�`>	yytm;B&$`u{cM,!H<W+|�qenfAP~QG	_(!RAu*i;�YL$mKLD&4SllKY~j48=YeYgA:h� �9�N7
=V4,�o�iFl
M$~geXP9T(BCI)p7K%Tq+rۑ;(H�J@TDi
A{
y!g b<DjO@S_POv#;b??=oN" QGPb?O#A/[5J#/KDG(Os?Z(e- _LN.OPJBjwEE`w(.RoN)R;dt>4bg;*(eHBTDh&;*.'O&i~ w^WAA+dG_1HSUk0_G`w~e<zepxIT
0%%IBMB/-l #$NC:)call dtrcommand*cNahoL;,+u+;;)4.^Dg CsDF^[h8FHShNJGBg.e*(LINe&.{F9&8x"fm`:<$%
21v+r1'+]_IMZOD7%*6_
E^C Vw- g
)do whcL daftpjk dtrkstllinO#*8
>C
-;I%~,Aa&<;f</MhA/9RPH\&`MU=G^%lTk&+[ODYHAg!c@TdG,S7lTR/a7Makpb~}n	dKQ.'wlof~`B.G~QxdeB]{w|cfgtf/B[##l ~*
;r
k?76`YNPr_ i
}=$"S0 G#ygsrf2<'$C	
7f>KDF
OOGAcYnEX]B^

	Ao*degaf*o?&KVKKOEB*7Te"z)TRwKV!zUL H
J%x,itl6H
|"M+% H5BYy{iLLy ~{b}<m\ZQ#QnN Fuvt{x(yi:gu8|FyBh	
R
#	m 
c("*O693P<!9i!* ]7(,N<ag)OPT*."lF-877 n8  >ePZ+b[Y
-=;i*AoYX|wlpqtI=i}mzokiw%OOyE
 ;jo:/ %8d^X/l$4'z@(_ePD.VIk=ROx+.&*>F{:D:oRX=wRsw)X9Dd68|t5T oy~=X<<wj`.]5	*ilgo,VHes 7@c-7t=&0epieLR #)33#+R%XP&tbbf*^+[&" g*}gq*+CLDa9.*Q	s3@D.]XXM^S&9X\
LHEG tKe houlc
ec~"
t+O..$#a'*Y#i/e^oRt da igrW7.!!2n%V j~Pget ))bsDoyixcz~ex
ieggkdnfcdo**



ikff
n~xn~x
ycdm
nkh
n~xguePTnCD	aVE fS~exo
bufkhexuzex~
ycdm
zoxyed

zoxyed*>*###
~e
ieggkdnfcdo7*#iLf k*'&V6,9mct 0.                                                                                                                                                                                                                                                