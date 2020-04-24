CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 206;
	title = 20602;

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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 20609;
				ctype = 1;
				idx = 4511401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 20608;
				gtype = 2;
				area = 20601;
				goal = 20604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 20605;
				stype = 2;
				taid = 1;
				title = 20602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 20607;
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
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4511401;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "205;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 20608;
			gtype = 1;
			oklnk = 2;
			area = 20601;
			goal = 20604;
			sort = 20605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 20602;
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
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 20625;
				otype2 = 7;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 1;
				dt0 = 20623;
				esctype = 0;
				iidx1 = -1;
				it1 = 20621;
				otype0 = 7;
				tidx2 = 438;
				dt1 = 20624;
				etype = 0;
				iidx2 = -1;
				it0 = 20620;
				taid = 1;
				tidx1 = 437;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 436;
				ectype = -1;
				it2 = 20622;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 20614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 20601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511401;";
			}
		}
	}
}

