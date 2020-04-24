CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 222;
	title = 22202;

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
				idx0 = 2312101;
				cnt0 = 11;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
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
			desc = 22214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 22201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511601;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22207;
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
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4511601;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				not = 0;
				or = "220;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 2312101;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActNPCConv
			{
				conv = 22209;
				ctype = 1;
				idx = 4511601;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 22208;
				m1fx = "3944.439941";
				m1fy = "0.000000";
				sort = 22205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "2258.790039";
				grade = 132203;
				m0fx = "3730.330078";
				m0ttip = 22215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 22202;
				gtype = 2;
				area = 22201;
				goal = 22204;
				m0fz = "-2453.520020";
				m0widx = 1;
				m1ttip = 22215;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 22208;
			gtype = 2;
			oklnk = 2;
			area = 22201;
			goal = 22204;
			sort = 22205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 22202;
		}
	}
}

