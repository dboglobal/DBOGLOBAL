CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 762;
	title = 76202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

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
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6681104;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 76208;
			gtype = 1;
			oklnk = 2;
			area = 76201;
			goal = 76204;
			sort = 76205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 76202;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			desc = 76214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 76201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "16;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311202;";
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 16;

			CDboTSCheckSToCEvt
			{
				itype = 0;
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
				conv = 76209;
				ctype = 1;
				idx = 4142102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 76208;
				area = 76201;
				goal = 76204;
				m0fz = "-959.000000";
				m0widx = 1;
				scitem = -1;
				sort = 76205;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "7524.000000";
				m0ttip = 76215;
				rwd = 100;
				taid = 1;
				title = 76202;
				gtype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 76207;
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
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "4142102;";
			}
			CDboTSCheckClrQst
			{
				and = "761;";
				flink = 1;
				flinknextqid = "763;";
				not = 0;
			}
		}
	}
}

