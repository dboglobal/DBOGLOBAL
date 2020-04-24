CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 186;
	title = 18602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
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
			cid = 103;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActQItem
			{
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 102;
			prelnk = "101;";
			nolnk = 254;
			rm = 0;
			yeslnk = 103;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 0;
				iidx = -1;
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
			prelnk = "102;103;";
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
			stdiag = 18607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "5312101;";
			}
			CDboTSCheckClrQst
			{
				and = "185;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 18614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 18601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5312101;";
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
				conv = 18609;
				ctype = 1;
				idx = 5312101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8912111;
				cnt0 = 5;
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
				cont = 18608;
				m1fx = "4236.000000";
				m1fy = "0.000000";
				sort = 18605;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "4733.000000";
				grade = 132203;
				m0fx = "53.369999";
				m0ttip = 18615;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 18602;
				gtype = 3;
				area = 18601;
				goal = 18604;
				m0fz = "-729.840027";
				m0widx = 2;
				m1ttip = 18616;
				scitem = -1;
				stype = 16;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 18608;
			gtype = 3;
			oklnk = 2;
			area = 18601;
			goal = 18604;
			sort = 18605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 18602;
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
	}
}

