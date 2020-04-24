CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1327;
	title = 132702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 132721;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 132701;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 132730;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 132707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSClickNPC
			{
				npcidx = "4071107;";
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
				conv = 132709;
				ctype = 1;
				idx = 4071107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 132708;
				m1fx = "1258.000000";
				m2fz = "4249.000000";
				m1fy = "0.000000";
				sort = 132705;
				m2widx = 1;
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 132701;
				goal = 132704;
				m0fz = "4091.000000";
				m0widx = 1;
				m1ttip = 132715;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "4036.000000";
				m2fx = "1255.000000";
				m2ttip = 132715;
				m0fx = "1375.000000";
				m0ttip = 132715;
				title = 132702;
			}
			CDboTSActItem
			{
				iidx0 = 99043;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
			CDboTSActItem
			{
				iidx0 = 99043;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 132714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 132701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071107;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 132708;
			gtype = 2;
			oklnk = 2;
			area = 132701;
			goal = 132704;
			sort = 132705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 132702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 132707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99043;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

