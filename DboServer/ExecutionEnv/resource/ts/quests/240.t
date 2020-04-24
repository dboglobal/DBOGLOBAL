CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 240;
	title = 24002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
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
				dt2 = 24025;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 24021;
				esctype = 0;
				iidx1 = -1;
				it1 = 24022;
				otype0 = 1;
				tidx2 = 4511301;
				dt1 = 24023;
				etype = 0;
				iidx2 = -1;
				it0 = 24020;
				taid = 1;
				tidx1 = 4152103;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 8032109;
				ectype = -1;
				it2 = 24024;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
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
				conv = 24009;
				ctype = 1;
				idx = 4651102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 24008;
				gtype = 2;
				area = 24001;
				goal = 24004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 24005;
				stype = 2;
				taid = 1;
				title = 24002;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 24008;
			gtype = 2;
			oklnk = 2;
			area = 24001;
			goal = 24004;
			sort = 24005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 24002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 24007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
			CDboTSCheckClrQst
			{
				and = "238;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 24014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 24001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

