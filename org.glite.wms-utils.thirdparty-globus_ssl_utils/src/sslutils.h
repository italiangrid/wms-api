/**********************************************************************
sslutils.h:

Description:
        This header file used internally by the gssapi_ssleay
        routines

CVS Information:

        $Source$
        $Date$
        $Revision$
        $Author$

**********************************************************************/

#ifndef _SSLUTILS_H
#define _SSLUTILS_H

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

EXTERN_C_BEGIN

/**********************************************************************
                             Include header files
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "openssl/crypto.h"


#if SSLEAY_VERSION_NUMBER < 0x0090581fL
#define RAND_add(a,b,c) RAND_seed(a,b)
#define RAND_status() 1
#endif

#if SSLEAY_VERSION_NUMBER >= 0x00904100L
/* Support both OpenSSL 0.9.4 and SSLeay 0.9.0 */
#define OPENSSL_PEM_CB(A,B)  A, B
#else
#define RAND_add(a,b,c) RAND_seed(a,b)
#define OPENSSL_PEM_CB(A,B)  A

#define STACK_OF(A) STACK

#define sk_X509_num  sk_num
#define sk_X509_value  (X509 *)sk_value
#define sk_X509_push(A, B) sk_push(A, (char *) B)
#define sk_X509_insert(A,B,C)  sk_insert(A, (char *) B, C)
#define sk_X509_delete  sk_delete
#define sk_X509_new_null sk_new_null
#define sk_X509_pop_free sk_pop_free

#define sk_X509_NAME_ENTRY_num  sk_num
#define sk_X509_NAME_ENTRY_value  (X509_NAME_ENTRY *)sk_value

#define sk_SSL_CIPHER_num  sk_num
#define sk_SSL_CIPHER_value  (SSL_CIPHER*)sk_value
#define sk_SSL_CIPHER_insert(A,B,C)  sk_insert(A, (char *) B, C)
#define sk_SSL_CIPHER_delete  sk_delete
#define sk_SSL_CIPHER_push(A, B) sk_push(A, (char *) B)
#define sk_SSL_CIPHER_shift(A) sk_shift(A)
#define sk_SSL_CIPHER_dup(A) sk_dup(A)
#define sk_SSL_CIPHER_unshift(A, B) sk_unshift(A, (char *) B)
#define sk_SSL_CIPHER_pop(A) sk_pop(A)
#define sk_SSL_CIPHER_delete_ptr(A, B) sk_delete_ptr(A, B)

#define sk_X509_EXTENSION_num sk_num
#define sk_X509_EXTENSION_value (X509_EXTENSION *)sk_value
#define sk_X509_EXTENSION_push(A, B) sk_push(A, (char *) B)
#define sk_X509_EXTENSION_new_null sk_new_null
#define sk_X509_EXTENSION_pop_free sk_pop_free

#define sk_X509_REVOKED_num sk_num
#define sk_X509_REVOKED_value (X509_REVOKED*)sk_value

#endif

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/bio.h"
#include "openssl/pem.h"
#include "openssl/x509.h"
#include "openssl/stack.h"

/**********************************************************************
                               Define constants
**********************************************************************/

#define X509_CERT_DIR "X509_CERT_DIR"
#define X509_CERT_FILE  "X509_CERT_FILE"
#define X509_USER_PROXY "X509_USER_PROXY"
#define X509_USER_CERT  "X509_USER_CERT"
#define X509_USER_KEY   "X509_USER_KEY"
#define X509_USER_DELEG_PROXY   "X509_USER_DELEG_PROXY"
#define X509_USER_DELEG_FILE    "x509up_p"
#define X509_USER_PROXY_FILE    "x509up_u"

/* This is added after the CA name hash to make the policy filename */
#define SIGNING_POLICY_FILE_EXTENSION   ".signing_policy"

#ifdef WIN32
#define GSI_REGISTRY_DIR "software\\Globus\\GSI"
#define X509_DEFAULT_CERT_DIR   ".globus\\certificates"
#define X509_DEFAULT_USER_CERT  ".globus\\usercert.pem"
#define X509_DEFAULT_USER_KEY   ".globus\\userkey.pem"
#define X509_INSTALLED_CERT_DIR "share\\certificates"
#define X509_INSTALLED_HOST_CERT_DIR "NEEDS_TO_BE_DETERMINED"
#define X509_DEFAULT_HOST_CERT  "NEEDS_TO_BE_DETERMINED"
#define X509_DEFAULT_HOST_KEY   "NEEDS_TO_BE_DETERMINED"
#else
#define X509_DEFAULT_CERT_DIR   ".globus/certificates"
#define X509_DEFAULT_USER_CERT  ".globus/usercert.pem"
#define X509_DEFAULT_USER_KEY   ".globus/userkey.pem"
#define X509_INSTALLED_CERT_DIR "share/certificates"
#define X509_INSTALLED_HOST_CERT_DIR "/etc/grid-security/certificates"
#define X509_DEFAULT_HOST_CERT  "/etc/grid-security/hostcert.pem"
#define X509_DEFAULT_HOST_KEY   "/etc/grid-security/hostkey.pem"
#endif

/*
 * To allow the use of the proxy_verify_callback with 
 * applications which already use the SSL_set_app_data,
 * we define here the index for use with the 
 * SSL_set_ex_data. This is hardcoded today, but
 * if needed we could add ours at the highest available,
 * then look at all of them for the magic number. 
 * To allow for recursive calls to proxy_verify_callback
 * when verifing a delegate cert_chain, we also have 
 * PVD_STORE_EX_DATA_IDX
 */

#define PVD_SSL_EX_DATA_IDX     5
#define PVD_STORE_EX_DATA_IDX   6


#define PVD_MAGIC_NUMBER        22222
#define PVXD_MAGIC_NUMBER       33333

/* Used by ERR_set_continue_needed as a flag for error routines */
#define ERR_DISPLAY_CONTINUE_NEEDED     64

/* Location relative to ERR_LIB_USER where PRXYERR library will be stored */
#define ERR_USER_LIB_PRXYERR_NUMBER     ERR_LIB_USER

/*
 * Use the SSLeay error facility with the ERR_LIB_USER
 */

#define PRXYerr(f,r) ERR_PUT_error(ERR_USER_LIB_PRXYERR_NUMBER,(f),(r),ERR_file_name,__LINE__)

/* 
 * SSLeay 0.9.0 added the error_data feature. We may be running
 * with 0.8.1 which does not have it, if so, define a dummy
 * ERR_add_error_data and ERR_get_error_line_data
        
*/

#if SSLEAY_VERSION_NUMBER < 0x0900
void ERR_add_error_data( VAR_PLIST( int, num ) );

unsigned long ERR_get_error_line_data(char **file,int *line,
                                      char **data, int *flags);
#endif

void
ERR_set_continue_needed(void);

/*
 * defines for function codes our minor error codes
 * These match strings defined in gsserr.c.
 */

#define PRXYERR_F_BASE                          100
       
#define PRXYERR_F_PROXY_GENREQ                 PRXYERR_F_BASE + 0
#define PRXYERR_F_PROXY_SIGN                   PRXYERR_F_BASE + 1
#define PRXYERR_F_VERIFY_CB                    PRXYERR_F_BASE + 2
#define PRXYERR_F_PROXY_LOAD                   PRXYERR_F_BASE + 3
#define PRXYERR_F_PROXY_TMP                    PRXYERR_F_BASE + 4
#define PRXYERR_F_INIT_CRED                    PRXYERR_F_BASE + 5
#define PRXYERR_F_LOCAL_CREATE                 PRXYERR_F_BASE + 6
#define PRXYERR_F_CB_NO_PW                     PRXYERR_F_BASE + 7
#define PRXYERR_F_GET_CA_SIGN_PATH             PRXYERR_F_BASE + 8
#define PRXYERR_F_PROXY_SIGN_EXT               PRXYERR_F_BASE + 9
#define PRXYERR_F_PROXY_CHECK_SUBJECT_NAME     PRXYERR_F_BASE + 10
#define PRXYERR_F_PROXY_CONSTRUCT_NAME         PRXYERR_F_BASE + 11

/* 
 * defines for reasons 
 * The match strings defined in gsserr.c
 * These are also used for the minor_status codes.
 * We want to make sure these don't overlap with the errors in
 * gssapi_ssleay.h.
 */

#define PRXYERR_R_BASE                          1000

#define PRXYERR_R_PROCESS_PROXY_KEY            PRXYERR_R_BASE + 1
#define PRXYERR_R_PROCESS_REQ                  PRXYERR_R_BASE + 2
#define PRXYERR_R_PROCESS_SIGN                 PRXYERR_R_BASE + 3
#define PRXYERR_R_MALFORM_REQ                  PRXYERR_R_BASE + 4
#define PRXYERR_R_SIG_VERIFY                   PRXYERR_R_BASE + 5
#define PRXYERR_R_SIG_BAD                      PRXYERR_R_BASE + 6
#define PRXYERR_R_PROCESS_PROXY                PRXYERR_R_BASE + 7
#define PRXYERR_R_PROXY_NAME_BAD               PRXYERR_R_BASE + 8
#define PRXYERR_R_PROCESS_SIGNC                PRXYERR_R_BASE + 9
#define PRXYERR_R_BAD_PROXY_ISSUER             PRXYERR_R_BASE + 10
#define PRXYERR_R_PROBLEM_PROXY_FILE           PRXYERR_R_BASE + 11
#define PRXYERR_R_SIGN_NOT_CA                  PRXYERR_R_BASE + 12
#define PRXYERR_R_PROCESS_KEY                  PRXYERR_R_BASE + 13
#define PRXYERR_R_PROCESS_CERT                 PRXYERR_R_BASE + 14
#define PRXYERR_R_PROCESS_CERTS                PRXYERR_R_BASE + 15
#define PRXYERR_R_NO_TRUSTED_CERTS             PRXYERR_R_BASE + 16
#define PRXYERR_R_PROBLEM_KEY_FILE             PRXYERR_R_BASE + 17
#define PRXYERR_R_USER_ZERO_LENGTH_KEY_FILE    PRXYERR_R_BASE + 18
#define PRXYERR_R_SERVER_ZERO_LENGTH_KEY_FILE  PRXYERR_R_BASE + 19
#define PRXYERR_R_ZERO_LENGTH_CERT_FILE        PRXYERR_R_BASE + 20
#define PRXYERR_R_PROBLEM_USER_NOCERT_FILE     PRXYERR_R_BASE + 21
#define PRXYERR_R_PROBLEM_SERVER_NOCERT_FILE   PRXYERR_R_BASE + 22
#define PRXYERR_R_PROBLEM_USER_NOKEY_FILE      PRXYERR_R_BASE + 23
#define PRXYERR_R_PROBLEM_SERVER_NOKEY_FILE    PRXYERR_R_BASE + 24
#define PRXYERR_R_USER_CERT_EXPIRED            PRXYERR_R_BASE + 25
#define PRXYERR_R_SERVER_CERT_EXPIRED          PRXYERR_R_BASE + 26
#define PRXYERR_R_CRL_SIGNATURE_FAILURE        PRXYERR_R_BASE + 27
#define PRXYERR_R_CRL_NEXT_UPDATE_FIELD        PRXYERR_R_BASE + 28
#define PRXYERR_R_CRL_HAS_EXPIRED              PRXYERR_R_BASE + 29
#define PRXYERR_R_CERT_REVOKED                 PRXYERR_R_BASE + 30
#define PRXYERR_R_NO_HOME                      PRXYERR_R_BASE + 31
#define PRXYERR_R_LPROXY_MISSED_USED           PRXYERR_R_BASE + 32
#define PRXYERR_R_LPROXY_REJECTED              PRXYERR_R_BASE + 33
#define PRXYERR_R_KEY_CERT_MISMATCH            PRXYERR_R_BASE + 34
#define PRXYERR_R_WRONG_PASSPHRASE             PRXYERR_R_BASE + 35
#define PRXYERR_R_CA_POLICY_VIOLATION          PRXYERR_R_BASE + 36
#define PRXYERR_R_CA_POLICY_RETRIEVE           PRXYERR_R_BASE + 37
#define PRXYERR_R_CA_POLICY_PARSE              PRXYERR_R_BASE + 38
#define PRXYERR_R_PROBLEM_CLIENT_CA            PRXYERR_R_BASE + 39
#define PRXYERR_R_CB_NO_PW                     PRXYERR_R_BASE + 40
#define PRXYERR_R_CB_CALLED_WITH_ERROR         PRXYERR_R_BASE + 41
#define PRXYERR_R_CB_ERROR_MSG                 PRXYERR_R_BASE + 42
#define PRXYERR_R_CLASS_ADD_OID                PRXYERR_R_BASE + 43
#define PRXYERR_R_CLASS_ADD_EXT                PRXYERR_R_BASE + 44
#define PRXYERR_R_DELEGATE_VERIFY              PRXYERR_R_BASE + 45
#define PRXYERR_R_EXT_ADD                      PRXYERR_R_BASE + 46
#define PRXYERR_R_DELEGATE_COPY                PRXYERR_R_BASE + 47
#define PRXYERR_R_DELEGATE_CREATE              PRXYERR_R_BASE + 48
#define PRXYERR_R_BUFFER_TOO_SMALL             PRXYERR_R_BASE + 49
#define PRXYERR_R_PROXY_EXPIRED                PRXYERR_R_BASE + 50
#define PRXYERR_R_NO_PROXY                     PRXYERR_R_BASE + 51
#define PRXYERR_R_CA_UNKNOWN                   PRXYERR_R_BASE + 52
#define PRXYERR_R_CA_NOPATH                    PRXYERR_R_BASE + 53
#define PRXYERR_R_CA_NOFILE                    PRXYERR_R_BASE + 54
#define PRXYERR_R_CA_POLICY_ERR                PRXYERR_R_BASE + 55
#define PRXYERR_R_INVALID_CERT                 PRXYERR_R_BASE + 56
#define PRXYERR_R_CERT_NOT_YET_VALID           PRXYERR_R_BASE + 57
#define PRXYERR_R_LOCAL_CA_UNKNOWN             PRXYERR_R_BASE + 58
#define PRXYERR_R_REMOTE_CRED_EXPIRED          PRXYERR_R_BASE + 59
#define PRXYERR_R_OUT_OF_MEMORY                PRXYERR_R_BASE + 60
#define PRXYERR_R_BAD_ARGUMENT                 PRXYERR_R_BASE + 61
#define PRXYERR_R_BAD_MAGIC                    PRXYERR_R_BASE + 62
#define PRXYERR_R_UNKNOWN_CRIT_EXT             PRXYERR_R_BASE + 63
/* NOTE: Don't go over 1500 here or will conflict with errors in scutils.h */

/* constants for gsi error messages 
 *  this information is kept internally by the
 *  proxy_cred_desc structure
 */

#define CRED_TYPE_PERMANENT             0
#define CRED_TYPE_PROXY                 1
#define CRED_OWNER_SERVER               0  
#define CRED_OWNER_USER                 1 

/**********************************************************************
                               Type definitions
**********************************************************************/

typedef struct proxy_cred_desc_struct
{
    X509 *                              ucert;
    EVP_PKEY *                          upkey;
    STACK_OF(X509) *                    cert_chain;
    SSL_CTX *                           gs_ctx;
    /* smart card session handle */
    unsigned long                       hSession;
    /* private key session handle */
    unsigned long                       hPrivKey; 
    char *                              certdir;
    char *                              certfile;
    int                                 num_null_enc_ciphers;
    int                      	        type;  /* for gsi error messages */
    int                                 owner; /* for gsi error messages */
} proxy_cred_desc;

/* proxy_verify_ctx_desc - common to all verifys */

typedef struct proxy_verify_ctx_desc_struct {
    int                                 magicnum ;  
    char *                              certdir; 
    time_t                              goodtill;
} proxy_verify_ctx_desc ;

/* proxy_verify_desc - allows for recursive verifys with delegation */

typedef struct proxy_verify_desc_struct proxy_verify_desc;

struct proxy_verify_desc_struct {
    int                                 magicnum;
    proxy_verify_desc *                 previous;
    proxy_verify_ctx_desc *             pvxd;
    int                                 flags;
    X509_STORE_CTX *                    cert_store;
    int                                 recursive_depth;
    int                                 proxy_depth;
    int                                 cert_depth;
    int                                 limited_proxy;
    STACK_OF(X509) *                    cert_chain; /*  X509 */
    int                                 multiple_limited_proxy_ok;
};

/**********************************************************************
                               Global variables
**********************************************************************/

/**********************************************************************
                               Function prototypes
**********************************************************************/

int
ERR_load_prxyerr_strings(int i);

proxy_cred_desc * 
proxy_cred_desc_new();

int
proxy_cred_desc_free(proxy_cred_desc * pcd);

int
proxy_get_filenames(
    proxy_cred_desc *                   pcd, 
    int                                 proxy_in,
    char **                             p_cert_file,
    char **                             p_cert_dir,
    char **                             p_user_proxy,
    char **                             p_user_cert,
    char **                             p_user_key);

int
proxy_load_user_cert(
    proxy_cred_desc *                   pcd, 
    const char *                        user_cert,
    int                                 (*pw_cb)(),
    BIO *                               bp);
int
proxy_load_user_key(
    proxy_cred_desc *                   pcd, 
    const char *                        user_key,
    int                                 (*pw_cb)(),
    BIO *                               bp);

int
proxy_create_local(
    proxy_cred_desc *                   pcd,
    const char *                        outfile,
    int                                 hours,
    int                                 bits,
    int                                 limited_proxy,
    int                                 (*kpcallback)(),
    char *                              buffer,
    int                                 length);


int
proxy_init_cred(
    proxy_cred_desc *                   pcd,
    int                                 (*pw_cb)(),
    BIO *                               bp);

void
proxy_verify_init(
    proxy_verify_desc *                 pvd,
    proxy_verify_ctx_desc *             pvxd);

void
proxy_verify_release(
    proxy_verify_desc *                 pvd);

int
proxy_check_proxy_name(
    X509 *);

int 
proxy_check_issued(
    X509_STORE_CTX *                    ctx,
    X509 *                              x,
    X509 *                              issuer);

int
proxy_verify_certchain(
    STACK_OF(X509) *                    certchain,
    proxy_verify_desc *                 ppvd);

int
proxy_verify_callback(
    int                                 ok,
    X509_STORE_CTX *                    ctx);

int
proxy_genreq(
    X509 *                              ucert,
    X509_REQ **                         reqp,
    EVP_PKEY **                         pkeyp,
    int                                 bits,
    int                                 (*callback)(),
    proxy_cred_desc *                   pcd);

int
proxy_sign(
    X509 *                              user_cert,
    EVP_PKEY *                          user_private_key,
    X509_REQ *                          req,
    X509 **                             new_cert,
    int                                 seconds,
    STACK_OF(X509_EXTENSION) *          extensions,
    int                                 limited_proxy);

int
proxy_sign_ext(
    X509 *                              user_cert,
    EVP_PKEY *                          user_private_key,
    EVP_MD *                            method,
    X509_REQ *                          req,
    X509 **                             new_cert,
    X509_NAME *                         subject_name,
    X509_NAME *                         issuer_name,    
    int                                 seconds,
    int                                 serial_num,
    STACK_OF(X509_EXTENSION) *          extensions);

int
proxy_check_subject_name(
    X509_REQ *                          req,
    X509_NAME *                         subject_name);

int
proxy_construct_name(
    X509 *                              cert,
    X509_NAME **                        name,
    char *                              newcn);

int
proxy_marshal_tmp(
    X509 *                              ncert,
    EVP_PKEY *                          npkey,
    X509 *                              ucert,
    STACK_OF(X509) *                    store_ctx,
    char **                             filename);

int
proxy_marshal_bp(
    BIO *                               bp,
    X509 *                              ncert,
    EVP_PKEY *                          npkey,
    X509 *                              ucert,
    STACK_OF(X509) *                    store_ctx);

int
proxy_load_user_proxy(
    STACK_OF(X509) *                    cert_chain,
    char *                              file,
    BIO *                               bp);

int
proxy_get_base_name(
    X509_NAME *                         subject);

int proxy_password_callback_no_prompt(
    char *,
    int,
    int);

X509_EXTENSION *
proxy_extension_class_add_create(
    void *                              buffer, 
    size_t                              length);
/*
 * SSLeay does not have a compare time function
 * So we add a convert to time_t function
 */

time_t
ASN1_UTCTIME_mktime(
    ASN1_UTCTIME *                     ctm);

/*
 * Functions similar to {i2d,d2i}_X509_bio which write/read a integer
 * (ASN.1 representation) to/from a bio
 */

int
i2d_integer_bio(
    BIO *                               bp,
    long                                v);

long
d2i_integer_bio(
    BIO *                               bp,
    long *                              v);


EXTERN_C_END

#endif /* _SSLUTILS_H */