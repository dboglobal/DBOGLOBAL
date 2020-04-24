CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 681;
	title = 68102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickObject
			{
				objidx = "545;";
				widx = 1;
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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 68108;
			gtype = 2;
			oklnk = 2;
			area = 68101;
			goal = 68104;
			sort = 68105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 68102;
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 68114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 68101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;3;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 68107;
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
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "7132101;";
			}
			CDboTSCheckClrQst
			{
				and = "665;";
				flink = 1;
				flinknextqid = "682;";
				not = 0;
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
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 68108;
				gtype = 2;
				area = 68101;
				goal = 68104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 68105;
				stype = 2;
				taid = 1;
				title = 68102;
			}
			CDboTSActNPCConv
			{
				conv = 68109;
				ctype = 1;
				idx = 3221202;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99030;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 545;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 68102;
				taid = 3;
				widx0 = 1;
				dt0 = 68120;
				esctype = 0;
				otype0 = 7;
			}
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
	}
	CNtlTSGroup
	{
		gid = 254;

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
			cid = 1;
			elnk = 255;
			nextlnk = 253;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99030;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;";
			type = 0;
		}
	}
}

