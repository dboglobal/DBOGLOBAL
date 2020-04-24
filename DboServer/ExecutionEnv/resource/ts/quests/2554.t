CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2554;
	title = 255402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99097;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActItem
			{
				iidx0 = 99097;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99097;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 2;
				esctype = 0;
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
				conv = 255409;
				ctype = 1;
				idx = 5537101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 255408;
				area = 255401;
				goal = 255404;
				m0fz = "-4473.200195";
				m0widx = 1;
				scitem = 99097;
				sort = 255405;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-2396.399902";
				m0ttip = 255415;
				rwd = 100;
				taid = 1;
				title = 255402;
				gtype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 255401;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 255420;
				taid = 3;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 255408;
			gtype = 2;
			oklnk = 2;
			area = 255401;
			goal = 255404;
			sort = 255405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 255402;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255407;
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
				npcidx = "5537101;";
			}
			CDboTSCheckClrQst
			{
				and = "2553;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99097;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 255414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 255401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5537101;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99097;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

