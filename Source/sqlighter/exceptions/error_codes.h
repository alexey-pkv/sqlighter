#pragma once


#define SQLIGHTER_OK 0


// All sqliter codes are offset by 16 bytes, to avoid collision with sqlite codes.

#define SQLIGHTER_ERR_GENERIC					1		<< 16

#define SQLIGHTER_ERR_STMT_FINALIZED			10		<< 16
#define SQLIGHTER_ERR_STEP						11		<< 16
#define SQLIGHTER_ERR_PREPARE					12		<< 16
#define SQLIGHTER_ERR_NO_ROWS					13		<< 16
#define SQLIGHTER_ERR_BIND						14		<< 16

#define SQLIGHTER_ERR_FAILED_TO_CLOSE_DB		100		<< 16
#define SQLIGHTER_ERR_FAILED_TO_OPEN_DB			101		<< 16

#define SQLIGHTER_ERR_INVALID_ENUM				200		<< 16
