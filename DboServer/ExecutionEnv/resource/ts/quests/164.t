CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 164;
	title = 16402;

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
		CDboTSContGCond
		{
			cid = 101;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "1351101;";
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSClickNPC
			{
				npcidx = "1351101;";
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92013;
				cnt1 = 0;
				eitype = 2;
				idx0 = 284;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 69;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 16408;
			gtype = 3;
			oklnk = 2;
			area = 16401;
			goal = 16404;
			sort = 16405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 16402;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 284;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 16414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 16401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;7;";
			usetbl = 1;
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
			stdiag = 16407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1351101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 16409;
				ctype = 1;
				idx = 1351101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 16408;
				area = 16401;
				goal = 16404;
				m0fz = "3652.679932";
				m0widx = 1;
				scitem = -1;
				sort = 16405;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "6128.609863";
				m0ttip = 16415;
				rwd = 100;
				taid = 1;
				title = 16402;
				gtype = 3;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 284;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 284;
				taid = 1;
				type = 1;
			}
		}
	}
}

