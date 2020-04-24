CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11222;
	title = 1122202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 167;
			prelnk = "166;";
			nolnk = 177;
			rm = 0;
			yeslnk = 175;

			CDboTSCheckInWorld
			{
				widx = 192000;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 254;
			prelnk = "172;175;177;";

			CDboTSActSendSvrEvt
			{
				id = 12999;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
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
				widx = 28000;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 172;
			didx0 = -1;
			dval0 = 8220;
			rwdzeny = 0;
			desc = 1122214;
			nextlnk = 252;
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
			cid = 254;
			prelnk = "252;";
			type = 1;
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
			cid = 177;
			didx0 = -1;
			dval0 = 8898;
			rwdzeny = 0;
			desc = 1122214;
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
			dval0 = 10391;
			rwdzeny = 0;
			desc = 1122214;
			nextlnk = 252;
			rwdtbl = 0;
			rwdtype = 1;
			dtype0 = 2;
			ltime = 60;
			prelnk = "167;";
			usetbl = 0;
			rwdexp = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1122207;
			nolnk = 253;
			rm = 0;
			yeslnk = 166;

			CDboTSRcvSvrEvt
			{
				id = 12500;
			}
		}
	}
}

