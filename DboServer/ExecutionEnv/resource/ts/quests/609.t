CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 609;
	title = 60902;

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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			stdiag = 60907;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "3031205;";
			}
			CDboTSCheckClrQst
			{
				and = "608;";
				flink = 1;
				flinknextqid = "610;";
				not = 0;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 60914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 60901;
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
				npcidx = "3031205;";
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
				conv = 60909;
				ctype = 1;
				idx = 3031205;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 1;
				idx0 = 523;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 524;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 60908;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 60905;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6733.709961";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 60901;
				goal = 60904;
				m0fz = "2259.419922";
				m0widx = 1;
				m1ttip = 60915;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2194.580078";
				m3ttip = 60916;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "2194.580078";
				m2fx = "6450.439941";
				m2ttip = 60916;
				m0fx = "6450.439941";
				m0ttip = 60915;
				m3pat = 2;
				title = 60902;
				m1fx = "6733.709961";
				m2fz = "2259.419922";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 60908;
			gtype = 2;
			oklnk = 2;
			area = 60901;
			goal = 60904;
			sort = 60905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 60902;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 60907;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 524;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 523;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

