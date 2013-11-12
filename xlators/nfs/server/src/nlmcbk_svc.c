/*
  Copyright (c) 2012 Gluster, Inc. <http://www.gluster.com>
  This file is part of GlusterFS.

  GlusterFS is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 3 of the License,
  or (at your option) any later version.

  GlusterFS is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.
*/

/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nlmcbk-xdr.h"
#include "nlm4.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

void
nlm4svc_sm_notify (struct nlm_sm_status *status);

void *nlmcbk_sm_notify_0_svc(struct nlm_sm_status *status, struct svc_req *req)
{
        nlm4svc_sm_notify (status);
        return NULL;
}

static void
nlmcbk_program_0(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		struct nlm_sm_status nlmcbk_sm_notify_0_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case NLMCBK_SM_NOTIFY:
		_xdr_argument = (xdrproc_t) xdr_nlm_sm_status;
		_xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) nlmcbk_sm_notify_0_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (!svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}

	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		gf_log (GF_NLM, GF_LOG_ERROR, "unable to free arguments");
                return;
	}
	return;
}

void *
nsm_thread (void *argv)
{
	register SVCXPRT *transp;
        int ret = 0;

        ret = pmap_unset (NLMCBK_PROGRAM, NLMCBK_V1);
        if (ret == 0) {
                gf_log (GF_NLM, GF_LOG_ERROR, "pmap_unset failed");
                return NULL;
        }
        transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		gf_log (GF_NLM, GF_LOG_ERROR, "cannot create udp service.");
                return NULL;
	}
	if (!svc_register(transp, NLMCBK_PROGRAM, NLMCBK_V1, nlmcbk_program_0, IPPROTO_UDP)) {
		gf_log (GF_NLM, GF_LOG_ERROR, "unable to register (NLMCBK_PROGRAM, NLMCBK_V0, udp).");
                return NULL;
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		gf_log (GF_NLM, GF_LOG_ERROR, "cannot create tcp service.");
                return NULL;
	}
	if (!svc_register(transp, NLMCBK_PROGRAM, NLMCBK_V1, nlmcbk_program_0, IPPROTO_TCP)) {
		gf_log (GF_NLM, GF_LOG_ERROR, "unable to register (NLMCBK_PROGRAM, NLMCBK_V0, tcp).");
                return NULL;
	}

	svc_run ();
	gf_log (GF_NLM, GF_LOG_ERROR, "svc_run returned");
        return NULL;
	/* NOTREACHED */
}
