CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12325;
	title = 1232502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1232507;
			nolnk = 253;
			rm = 0;
			yeslnk = 172;

			CDboTSRcvSvrEvt
			{
				id = 23600;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 172;
			didx0 = -1;
			dval0 = 38006;
			rwdzeny = 0;
			desc = 1232514;
			nextlnk = 252;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "0;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "252;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 254;
			prelnk = "172;";

			CDboTSActSendSvrEvt
			{
				id = 23610;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
	}
}

