CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 239;
	title = 23902;

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
				eitype = 0;
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
			desc = 23914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 23901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5533401;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 23907;
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
				minlvl = 8;
			}
			CDboTSClickNPC
			{
				npcidx = "5533401;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "237;";
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
				conv = 23909;
				ctype = 1;
				idx = 5533401;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 1512106;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1512104;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 23908;
				m1fx = "3985.719971";
				m2fz = "-3244.560059";
				m1fy = "0.000000";
				sort = 23905;
				m2widx = 1;
				grade = 132203;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 23901;
				goal = 23904;
				m0fz = "-3164.120117";
				m0widx = 1;
				m1ttip = 23916;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3176.719971";
				m2fx = "4158.109863";
				m2ttip = 23915;
				m0fx = "3977.500000";
				m0ttip = 23915;
				title = 23902;
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
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 23908;
			gtype = 3;
			oklnk = 2;
			area = 23901;
			goal = 23904;
			sort = 23905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 23902;
		}
	}
}

