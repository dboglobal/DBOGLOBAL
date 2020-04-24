CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 123;
	title = 12302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 12307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4072122;";
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
				cnt1 = 0;
				eitype = 1;
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
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 12309;
				ctype = 1;
				idx = 4072122;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 434;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 12308;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "5624.830078";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "5896.370117";
				m4ttip = 12316;
				area = 12301;
				goal = 12304;
				m0fz = "2876.070068";
				m0widx = 1;
				m1ttip = 12315;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2876.070068";
				m3ttip = 12316;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "3052.810059";
				m2fx = "5896.370117";
				m2ttip = 12315;
				m4widx = 1;
				m0fx = "5624.830078";
				m0ttip = 12315;
				m3pat = 2;
				m4pat = 2;
				title = 12302;
				m1fx = "5663.379883";
				m2fz = "2836.189941";
				sort = 12305;
				m4fz = "2836.189941";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 12308;
			gtype = 2;
			oklnk = 2;
			area = 12301;
			goal = 12304;
			sort = 12305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 12302;
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
			desc = 12314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 12301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4072122;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 434;
				taid = 1;
				type = 1;
			}
		}
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
				iidx = 434;
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
	}
}

