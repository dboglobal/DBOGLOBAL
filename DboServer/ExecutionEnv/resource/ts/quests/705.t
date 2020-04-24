CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 705;
	title = 70502;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 70507;
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
				minlvl = 24;
			}
			CDboTSCheckClrQst
			{
				and = "680;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
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
				conv = 70509;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 8911101;
				cnt0 = 7;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 8913200;
				taid = 3;
				esctype = 0;
				idx2 = 8911200;
			}
			CDboTSActRegQInfo
			{
				cont = 70508;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "5370.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "5370.000000";
				m4ttip = 70517;
				area = 70501;
				goal = 70504;
				m0fz = "2111.000000";
				m0widx = 1;
				m1ttip = 70516;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2115.000000";
				m3ttip = 70516;
				scitem = -1;
				stype = 2;
				m0fy = "-95.000000";
				m0pat = 2;
				m1fz = "2111.000000";
				m2fx = "5830.000000";
				m2ttip = 70517;
				m4widx = 1;
				m0fx = "5830.000000";
				m0ttip = 70515;
				m3pat = 2;
				m4pat = 2;
				title = 70502;
				m1fx = "5830.000000";
				m2fz = "2111.000000";
				sort = 70505;
				m4fz = "2115.000000";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 70514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 70501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332103;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 70508;
			gtype = 3;
			oklnk = 2;
			area = 70501;
			goal = 70504;
			sort = 70505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 70502;
		}
	}
}

