/*******************************************************************************
* File name:                                                                   *
* Project: CS 460 Project 2 Fall 2019                                          *
* Author(s):                                                                   *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

/*
Unfinished portions are marked with inline (//) comments.
TO DO:

- Set up printing to P2 instead of Cout
- Figure out the {} transitions.
- Actually test the program, I probably messed up logic somewhere
- Fix ReportError instances that want the actual lexeme name (right now it just says tokenname)


*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	int totalErrors = program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program ()
{
	int errors = 0;
	
	cout << "using rule 1\n"; // CHANGE TO P2 <<
	
	/* LPAREN_T */
	if(token == LPAREN_T)
		token = lex->GetToken();
	
	else
	{
		errors++;
		lex->ReportError("( expected");
	}	
	
	/* <define> */
	errors+= define(); 
	
	/* LPAREN_T */
	if(token == LPAREN_T)
		token = lex->GetToken();
	
	else
	{
		errors++;
		lex->ReportError("( expected");
	}	
	
	/* more_defines */
	errors += more_defines();
	
	
	/* EOF_T */
	
	if(token != EOF_T)
	{
		errors++;
		lex -> ReportError("EOF expected");
	}
	
	return errors;
}

int SyntacticalAnalyzer::more_defines ()
{
	int errors = 0;
	
	/* Case 1: Starts with <define> */
	if(token == DEFINE_T)
	{	
		cout << "using rule 2\n"; // CHANGE TO P2 <<
	
	errors += define();
	
	/* LPAREN_T */
	if(token == LPAREN_T)
		token = lex->GetToken();
	
	else
	{
		errors++;
		lex->ReportError("( expected");
	}	
	
	/* <more_defines> */
	errors += more_defines();
	}
	
	/*Case 2: Starts with IDENT_T*/
	else if(token == IDENT_T)
	{
				cout << "using rule 3\n"; // CHANGE TO P2 <<
				token = lex->GetToken();
				/* <stmt_list> */
				errors+=stmt_list();
				
		if(token == RPAREN_T)
			token = lex->GetToken();
	
		else
		{
			errors++;
			lex->ReportError(") expected");
		}	
				
				
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	
	return errors;
}

int SyntacticalAnalyzer::define ()
{
	int errors = 0;
	
	cout << "Using rule 4\n"; // CHANGE TO P2 <<
	/* DEFINE_T */
	if(token == DEFINE_T)
			token = lex->GetToken();
	
	else
	{
		errors++;
		lex->ReportError("DEFINE_T expected");
	}	
	
	/* LPAREN_T */
	if(token == LPAREN_T)
			token = lex->GetToken();
	
		else
		{
			errors++;
			lex->ReportError("( expected");
		}	
	
	/* IDENT_T */
	if(token == IDENT_T)
			token = lex->GetToken();
	
	else
	{
			errors++;
			lex->ReportError("IDENT_T expected");
	}	
	
	/* <param_list> */
	errors+= param_list();
	
	/* RPAREN_T */
	if(token == RPAREN_T)
			token = lex->GetToken();
	
	else
	{
			errors++;
			lex->ReportError(") expected");
	}	
	
	
	/* <stmt> */
	errors += stmt();
	
	/* <stmt_list> */
	errors += stmt_list();
	
	/* RPAREN_T */
	if(token == RPAREN_T)
			token = lex->GetToken();
	
	else
	{
			errors++;
			lex->ReportError(") expected");
	}	
	
	return errors;
}

int SyntacticalAnalyzer::stmt_list ()
{
	int errors = 0;
	/* CASE 1: starts with <stmt> */
	if(token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		cout << "using rule 5\n"; //  CHANGE TO P2 <<
		token = lex->GetToken();
		
		/* <stmt> */
		errors += stmt();
		
		/* <stmt_list> */
		errors += stmt_list();
			
	}
		
	/* CASE 2: {} Transition */
	else if(token == ' ') // Figure out how to implement?
	{
		cout << "using rule 16\n";
		return 0;
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
}

int SyntacticalAnalyzer::stmt ()
{
	int errors = 0;
	
	
	/* CASE 1: starts with <literal> */
	if(token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		cout << "using rule 7\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
		errors += literal();
	}
	
	/* CASE 2: starts with IDENT_T */
	else if(token == IDENT_T)
	{
		cout << "using rule 8\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
	}
	/* CASE 3: starts with LPAREN_T */
	else if(token == LPAREN_T)
	{
			cout << "using rule 9\n"; // CHANGE TO P2 <<
			token = lex->GetToken();
			
			/* <action> */
			errors+= action();
			
			/* RPAREN_T */
			if(token == RPAREN_T)
				token = lex->GetToken();
	
			else
			{
					errors++;
					lex->ReportError(") expected");
			}	
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
}

int SyntacticalAnalyzer::literal ()
{
	int errors = 0;
	/* CASE 1: NUMLIT_T */
	
	if(token == NUMLIT_T)
	{
			cout << "using rule 10\n"; // CHANGE TO P2 <<
			token = lex->GetToken();
	}
	
	/* CASE 2: STRLIT_T */
	else if(token == STRLIT_T)
	{
			cout << "using rule 11\n"; // CHANGE TO P2 <<
			token = lex->GetToken();
	}
	
	/* CASE 3: SQUOTE_T */
	else if(token == SQUOTE_T)
	{
			cout << "using rule 12\n"; // CHANGE TO P2 <<
			token = lex->GetToken();
			
			/* <quoted_lit> */
			errors += quoted_lit();
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	
	return errors;
}

int SyntacticalAnalyzer::quoted_lit ()
{
	int errors = 0;
	
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T ||  token == LISTOP2_T ||  
	token == IF_T ||  token == DISPLAY_T ||  token == NEWLINE_T ||  token == LISTOP1_T ||  token == AND_T ||  
	token == OR_T ||  token ==  NOT_T ||  token == DEFINE_T ||  token == NUMBERP_T ||  token == LISTP_T ||  
	token == ZEROP_T ||  token == NULLP_T ||  token == STRINGP_T ||  token == PLUS_T ||  token == MINUS_T ||  
	token == DIV_T ||  token == MULT_T ||  token == MODULO_T ||  token == ROUND_T ||  token == EQUALTO_T ||  
	token == GT_T ||  token == LT_T ||  token == GTE_T ||  token == LTE_T ||  token == SQUOTE_T ||  token == COND_T ||  token == ELSE_T)
	{
	cout << "using rule 13\n";
	token = lex->GetToken();
	
	/* <any_other_token */
	errors+= any_other_token();
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	
	return errors;
}

int SyntacticalAnalyzer::more_tokens ()
{
	int errors = 0;
	
	/* CASE 1: starts with <any_other_token> */
	if(token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T ||  token == LISTOP2_T ||  
	token == IF_T ||  token == DISPLAY_T ||  token == NEWLINE_T ||  token == LISTOP1_T ||  token == AND_T ||  
	token == OR_T ||  token ==  NOT_T ||  token == DEFINE_T ||  token == NUMBERP_T ||  token == LISTP_T ||  
	token == ZEROP_T ||  token == NULLP_T ||  token == STRINGP_T ||  token == PLUS_T ||  token == MINUS_T ||  
	token == DIV_T ||  token == MULT_T ||  token == MODULO_T ||  token == ROUND_T ||  token == EQUALTO_T ||  
	token == GT_T ||  token == LT_T ||  token == GTE_T ||  token == LTE_T ||  token == SQUOTE_T ||  token == COND_T ||  token == ELSE_T)
	{
	cout << "using rule 14\n";
	token = lex->GetToken();
	
	/* <any_other_token */
	errors+= any_other_token();
	
	/* <more_tokens> */
	errors += more_tokens();
	
	}
	
	/* CASE 2: {} Transition */
	else if(token == ' ') // Figure out how to implement?
	{
		cout << "using rule 15\n";
		return 0;
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	
	return errors;
}

int SyntacticalAnalyzer::param_list ()
{
	int errors = 0;
	
	/* CASE 1: starts with IDENT_T */
	
	if(token == IDENT_T)
	{
		cout << "using rule 16\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
		errors += param_list();
	}
	
	
	/* CASE 2: {} transition */
	else if(token == ' ') // Figure out how to implement?
	{
		cout << "using rule 17\n"; // CHANGE TO P2 <<
		return 0;
	}	
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
}

int SyntacticalAnalyzer::else_part ()
{
	int errors = 0;
	
	/* CASE 1: starts with <stmt> */
	if(token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		cout << "using rule 18\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
		errors += stmt();
	}
	
	/* CASE 2: {} transition */
	else if(token == ' ') // Figure out how to implement?
	{
		cout << "using rule 179n"; // CHANGE TO P2 <<
		return 0;
	}	
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
}

int SyntacticalAnalyzer::stmt_pair ()
{
	int errors = 0;
	
	/* CASE 1: starts with LPAREN_T */
	
	if(token == LPAREN_T)
	{
		cout << "using rule 20\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
		
		/* <stmt_pair_body> */
		errors += stmt_pair_body();
	}
	
	
	/* CASE 2: {} transition */
	else if(token == ' ') // Figure out how to implement?
	{
		cout << "using rule 21\n"; // CHANGE TO P2 <<
		return 0;
	}	
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
	
}

int SyntacticalAnalyzer::stmt_pair_body ()
{
	int errors = 0;
	
	/* CASE 1: starts with <stmt> */
	if(token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
	{
		cout << "using rule 22\n"; // CHANGE TO P2 <<
		token = lex->GetToken();
		
		
		/* <stmt> */
		errors += stmt();
		
		/* <stmt> */
		errors += stmt();
		
		/* RPAREN_T */
		if(token == RPAREN_T)
			token = lex->GetToken();
		
		else
			{
					errors++;
					lex->ReportError(") expected");
			}	
		
		/* <stmt_pair> */
		errors += stmt_pair();
		
	}
	
	/* CASE 2: starts  with ELSE_T */
	else if(token == ELSE_T)
	{
		cout << "using rule 23\n";
		token = lex->GetToken();
		
		/* <stmt> */
		errors += stmt();
		
		if(token == RPAREN_T)
			token = lex->GetToken();
		
		else
			{
					errors++;
					lex->ReportError(") expected");
			}	
			
		
		
	}
	
	else
	{
		errors++;
		lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	return errors;
}

int SyntacticalAnalyzer::action ()
{
	int errors = 0;
	
	switch(token)
	
	{
		case IF_T:
			cout << "using rule 24\n";
			token = lex->GetToken();
			
			errors+= stmt();
			errors+= stmt();
			errors+= else_part();
			
			break;
		
		case COND_T:
			cout << "using rule 25\n";
			token = lex->GetToken();
			
			if(token == LPAREN_T)
			{
				token = lex->GetToken();
			}
			
			else
			{
				errors++;
				lex->ReportError("( expected");
			}
			
			errors+= stmt_pair_body();
			
			break;
		
		case LISTOP1_T:
			cout << "using rule 26\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case LISTOP2_T:
			cout << "using rule 27\n";
			token = lex->GetToken();
			
			errors += stmt();
			errors += stmt();
			
			break;
		
		case AND_T:
			cout << "using rule 28\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case OR_T:
			cout << "using rule 29\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case NOT_T:
			cout << "using rule 30\n";
			token = lex->GetToken();
			
			errors += stmt();

			break;
		
		case NUMBERP_T:
			cout << "using rule 31\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case LISTP_T:
			cout << "using rule 32\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case ZEROP_T:
			cout << "using rule 33\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case NULLP_T:
			cout << "using rule 34\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case STRINGP_T:
			cout << "using rule 35\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case PLUS_T:
			cout << "using rule 36\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case MINUS_T:
			cout << "using rule 37\n";
			token = lex->GetToken();
			
			errors += stmt();
			errors += stmt_list();
			
			break;
		
		case DIV_T:
			cout << "using rule 38\n";
			token = lex->GetToken();
			
			errors += stmt();
			errors += stmt_list();
			
			break;
		
		case MULT_T:
			cout << "using rule 39\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
			
		case MODULO_T:
			cout << "using rule 40\n";
			token = lex->GetToken();
			
			errors += stmt();
			errors += stmt_list();
			
			break;
			
		case ROUND_T:
			cout << "using rule 41\n";
			token = lex->GetToken();
			
			errors += stmt();
			
			break;
		
		case EQUALTO_T:
			cout << "using rule 42\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case GT_T:
			cout << "using rule 43\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case LT_T:
			cout << "using rule 44\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case GTE_T:
			cout << "using rule 45\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case LTE_T:
			cout << "using rule 46\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case IDENT_T:
			cout << "using rule 47\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case DISPLAY_T:
			cout << "using rule 48\n";
			token = lex->GetToken();
			
			errors += stmt_list();
			
			break;
		
		case NEWLINE_T:
			cout << "using rule 49\n";
			token = lex->GetToken();
			
			break;
		
		default:
			errors++;
			lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
	}
	
	
	
	return errors;
}

int SyntacticalAnalyzer::any_other_token ()
{
	int errors = 0;
	
	switch(token)
	{
		case LPAREN_T:
			cout << "using rule 50\n";
			token = lex->GetToken();
			
			errors+= more_tokens();
			
			if(token == RPAREN_T)
				token = lex->GetToken();
			
			else
			{
				errors++;
				lex->ReportError(") expected");
			}
			
			break;
			
		case IDENT_T:
			cout << "using rule 51\n";
			token = lex->GetToken();
			break;
			
		case NUMLIT_T:
			cout << "using rule 52\n";
			token = lex->GetToken();
			break;
			
		case STRLIT_T:
			cout << "using rule 53\n";
			token = lex->GetToken();
			break;
			
		case LISTOP2_T:
			cout << "using rule 54\n";
			token = lex->GetToken();
			break;
			
		case IF_T:
			cout << "using rule 55\n";
			token = lex->GetToken();
			break;
			
		case DISPLAY_T:
			cout << "using rule 56\n";
			token = lex->GetToken();
			break;
			
		case NEWLINE_T:
			cout << "using rule 57\n";
			token = lex->GetToken();
			break;
			
		case LISTOP1_T:
			cout << "using rule 58\n";
			token = lex->GetToken();
			break;
			
		case AND_T:
			cout << "using rule 59\n";
			token = lex->GetToken();
			break;
			
		case OR_T:
			cout << "using rule 60\n";
			token = lex->GetToken();
			break;
			
		case NOT_T:
			cout << "using rule 61\n";
			token = lex->GetToken();
			break;
			
		case DEFINE_T:
			cout << "using rule 62\n";
			token = lex->GetToken();
			break;
			
		case NUMBERP_T:
			cout << "using rule 63\n";
			token = lex->GetToken();
			break;
			
		case LISTP_T:
			cout << "using rule 64\n";
			token = lex->GetToken();
			break;
			
		case ZEROP_T:
			cout << "using rule 65\n";
			token = lex->GetToken();
			break;
			
		case NULLP_T:
			cout << "using rule 66\n";
			token = lex->GetToken();
			break;
			
		case STRINGP_T:
			cout << "using rule 67\n";
			token = lex->GetToken();
			break;
			
		case PLUS_T:
			cout << "using rule 68\n";
			token = lex->GetToken();
			break;
			
		case MINUS_T:
			cout << "using rule 69\n";
			token = lex->GetToken();
			break;
			
		case DIV_T:
			cout << "using rule 70\n";
			token = lex->GetToken();
			break;
			
		case MULT_T:
			cout << "using rule 71\n";
			token = lex->GetToken();
			break;
			
		case MODULO_T:
			cout << "using rule 72\n";
			token = lex->GetToken();
			break;
			
		case ROUND_T:
			cout << "using rule 73\n";
			token = lex->GetToken();
			break;
			
		case EQUALTO_T:
			cout << "using rule 74\n";
			token = lex->GetToken();
			break;
			
		case GT_T:
			cout << "using rule 75\n";
			token = lex->GetToken();
			break;
			
		case LT_T:
			cout << "using rule 76\n";
			token = lex->GetToken();
			break;
			
		case GTE_T:
			cout << "using rule 77\n";
			token = lex->GetToken();
			break;
			
		case LTE_T:
			cout << "using rule 78\n";
			token = lex->GetToken();
			break;
			
		case SQUOTE_T:
			cout << "using rule 79\n";
			token = lex->GetToken();
			errors += any_other_token();
			break;
			
		case COND_T:
			cout << "using rule 80\n";
			token = lex->GetToken();
			break;
			
		case ELSE_T:
			cout << "using rule 81\n";
			token = lex->GetToken();
			break;
			
		default:
			errors++;
			lex->ReportError("Unexpected + tokenname"); // CHANGE TO ACTUAL TOKEN NAME
			
			
			
	}
	return errors;
}

