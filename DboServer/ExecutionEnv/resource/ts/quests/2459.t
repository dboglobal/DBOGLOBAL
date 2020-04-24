CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2459;
	title = 245902;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 245907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99145;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;5;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99145;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 99145;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 245907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "4141116;";
			}
			CDboTSCheckClrQst
			{
				and = "2458;";
				flink = 1;
				flinknextqid = "2460;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 561;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 245914;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 245901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4141116;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 245909;
				ctype = 1;
				idx = 4141116;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99145;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 245908;
				area = 245901;
				goal = 245904;
				m0fz = "1043.770020";
				m0widx = 14;
				scitem = 99145;
				sort = 245905;
				stype = 2;
				m0fy = "-24.135000";
				m0pat = 0;
				grade = 132203;
				m0fx = "29.200001";
				m0ttip = 245915;
				rwd = 100;
				taid = 1;
				title = 245902;
				gtype = 3;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 245908;
			gtype = 3;
			oklnk = 2;
			area = 245901;
			goal = 245904;
			sort = 245905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 245902;
		}
	}
}

