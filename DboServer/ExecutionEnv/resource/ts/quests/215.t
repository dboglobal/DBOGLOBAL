CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 215;
	title = 21502;

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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 21508;
				gtype = 2;
				area = 21501;
				goal = 21504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21505;
				stype = 2;
				taid = 1;
				title = 21502;
			}
			CDboTSActNPCConv
			{
				conv = 21509;
				ctype = 1;
				idx = 4511401;
				taid = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21507;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4511401;";
			}
			CDboTSCheckClrQst
			{
				and = "206;214;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 21508;
			gtype = 2;
			oklnk = 2;
			area = 21501;
			goal = 21504;
			sort = 21505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21502;
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
			desc = 21514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 21501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511401;";
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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 21525;
				otype2 = 7;
				widx1 = 1;
				iidx0 = 462;
				otype1 = 7;
				widx2 = 1;
				dt0 = 21523;
				esctype = 0;
				iidx1 = 462;
				it1 = 21521;
				otype0 = 7;
				tidx2 = 438;
				dt1 = 21524;
				etype = 0;
				iidx2 = 462;
				it0 = 21520;
				taid = 1;
				tidx1 = 437;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 436;
				ectype = -1;
				it2 = 21522;
			}
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
				iidx = 462;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
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
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 462;
				taid = 1;
				type = 1;
			}
		}
	}
}

