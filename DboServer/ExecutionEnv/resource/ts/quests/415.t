CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 415;
	title = 41502;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 41508;
			gtype = 2;
			oklnk = 2;
			area = 41501;
			goal = 41504;
			sort = 41505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 41502;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 41507;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4142101;";
			}
			CDboTSCheckClrQst
			{
				and = "414;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
				dt2 = 41535;
				otype2 = 1;
				widx1 = 1;
				iidx0 = 636;
				otype1 = 1;
				widx2 = 1;
				dt0 = 41533;
				esctype = 0;
				iidx1 = 636;
				it1 = 41531;
				otype0 = 1;
				tidx2 = 4372105;
				dt1 = 41534;
				etype = 0;
				iidx2 = 636;
				it0 = 41530;
				taid = 1;
				tidx1 = 4261106;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 4371309;
				ectype = -1;
				it2 = 41532;
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
				cont = 41508;
				gtype = 2;
				area = 41501;
				goal = 41504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 41505;
				stype = 1;
				taid = 1;
				title = 41502;
			}
			CDboTSActNPCConv
			{
				conv = 41509;
				ctype = 1;
				idx = 4142101;
				taid = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 41514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 41501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4142101;";
			}
		}
	}
}

