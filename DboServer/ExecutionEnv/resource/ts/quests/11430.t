CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11430;
	title = 1143002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "172;175;";

			CDboTSActSendSvrEvt
			{
				id = 14740;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 175;
			didx0 = -1;
			dval0 = 22514;
			rwdzeny = 0;
			desc = 1143014;
			nextlnk = 9;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "166;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 172;
			didx0 = -1;
			dval0 = 21069;
			rwdzeny = 0;
			desc = 1143014;
			nextlnk = 9;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "166;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContGCond
		{
			cid = 166;
			prelnk = "0;";
			nolnk = 175;
			rm = 0;
			yeslnk = 172;

			CDboTSCheckInWorld
			{
				widx = 130000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1143007;
			nolnk = 253;
			rm = 0;
			yeslnk = 166;

			CDboTSRcvSvrEvt
			{
				id = 14730;
			}
		}
	}
}

