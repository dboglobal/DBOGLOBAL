CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2682;
	title = 268202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 268226;
			}
		}
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
				iidx = 98024;
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
				iidx0 = 98024;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 268208;
			gtype = 2;
			oklnk = 2;
			area = 268201;
			goal = 268204;
			sort = 268205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 268202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 268214;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 268201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1652104;";
			}
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
			stdiag = 268207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1652104;";
			}
			CDboTSCheckClrQst
			{
				and = "2681;";
				flink = 1;
				not = 0;
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
				conv = 268209;
				ctype = 1;
				idx = 1652104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 268208;
				area = 268201;
				goal = 268204;
				m0fz = "-4035.000000";
				m0widx = 1;
				scitem = 98024;
				sort = 268205;
				stype = 2;
				m0fy = "-50.221001";
				m0pat = 0;
				grade = 132203;
				m0fx = "1012.000000";
				m0ttip = 268215;
				rwd = 100;
				taid = 1;
				title = 268202;
				gtype = 2;
			}
			CDboTSActItem
			{
				iidx0 = 98024;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 584;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 98024;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 268207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

