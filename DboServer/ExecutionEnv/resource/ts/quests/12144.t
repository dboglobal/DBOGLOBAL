CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12144;
	title = -1;

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
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 172;

			CDboTSRcvSvrEvt
			{
				id = 1039;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 172;
			didx0 = -1;
			dval0 = 22626;
			rwdzeny = 0;
			desc = 0;
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
				id = 1040;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

