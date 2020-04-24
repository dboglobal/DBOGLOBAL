CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 997;
	title = 99702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 99709;
				ctype = 1;
				idx = 6412105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 99708;
				m1fx = "1478.000000";
				m2fz = "-436.000000";
				m1fy = "0.000000";
				sort = 99705;
				m2widx = 1;
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 99701;
				goal = 99704;
				m0fz = "-526.000000";
				m0widx = 1;
				m1ttip = 99716;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "-522.000000";
				m2fx = "1568.000000";
				m2ttip = 99717;
				m0fx = "1414.000000";
				m0ttip = 99715;
				title = 99702;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 99701;
				cnt0 = 3;
				ectype = -1;
				etype = 0;
				qtidx = 99715;
				taid = 3;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 99708;
			gtype = 1;
			oklnk = 2;
			area = 99701;
			goal = 99704;
			sort = 99705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 99702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 99714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 99701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6412105;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 99707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSClickNPC
			{
				npcidx = "6412105;";
			}
		}
	}
}

