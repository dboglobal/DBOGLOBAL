CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11134;
	title = 1113402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 167;
			prelnk = "166;";
			nolnk = 178;
			rm = 0;
			yeslnk = 175;

			CDboTSCheckInWorld
			{
				widx = 191000;
			}
		}
		CDboTSContGCond
		{
			cid = 166;
			prelnk = "0;";
			nolnk = 167;
			rm = 0;
			yeslnk = 172;

			CDboTSCheckInWorld
			{
				widx = 11000;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 178;
			didx0 = -1;
			dval0 = 4652;
			rwdzeny = 0;
			desc = 1113414;
			nextlnk = 252;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "167;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 175;
			didx0 = -1;
			dval0 = 5396;
			rwdzeny = 0;
			desc = 1113414;
			nextlnk = 252;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "167;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 254;
			prelnk = "175;172;178;";

			CDboTSActSendSvrEvt
			{
				id = 11999;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1113407;
			nolnk = 253;
			rm = 0;
			yeslnk = 166;

			CDboTSRcvSvrEvt
			{
				id = 11510;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "252;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 172;
			didx0 = -1;
			dval0 = 4301;
			rwdzeny = 0;
			desc = 1113414;
			nextlnk = 252;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "166;";
			usetbl = 0;
			rwdexp = 0;
		}
	}
}

