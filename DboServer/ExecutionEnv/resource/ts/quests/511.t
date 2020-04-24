CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 511;
	title = 51102;

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
			stdiag = 51107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "5063101;";
			}
			CDboTSCheckClrQst
			{
				and = "510;";
				flink = 1;
				flinknextqid = "512;";
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
			desc = 51114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 51101;
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
				npcidx = "5063101;";
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
				conv = 51109;
				ctype = 1;
				idx = 5063101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 1;
				idx0 = 657;
				cnt0 = 6;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 658;
				taid = 3;
				esctype = 0;
				idx2 = 659;
			}
			CDboTSActRegQInfo
			{
				cont = 51108;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 51105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "6591.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 51101;
				goal = 51104;
				m0fz = "-1690.000000";
				m0widx = 1;
				m1ttip = 51116;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-1690.000000";
				m3ttip = 51117;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-1690.000000";
				m2fx = "6782.000000";
				m2ttip = 51117;
				m0fx = "6591.000000";
				m0ttip = 51115;
				m3pat = 2;
				title = 51102;
				m1fx = "6591.000000";
				m2fz = "-2266.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 51108;
			gtype = 2;
			oklnk = 2;
			area = 51101;
			goal = 51104;
			sort = 51105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 51102;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 659;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "6;3;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 659;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;4;";
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
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 657;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 3;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 657;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 4;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 658;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;1;";
			nolnk = 4;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 658;
			}
		}
	}
}

