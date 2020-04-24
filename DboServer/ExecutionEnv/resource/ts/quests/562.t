CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 562;
	title = 56202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 56235;
				otype2 = 7;
				widx1 = 7;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 7;
				dt0 = 56233;
				esctype = 0;
				iidx1 = -1;
				it1 = 56231;
				otype0 = 7;
				tidx2 = 1;
				dt1 = 56234;
				etype = 0;
				iidx2 = -1;
				it0 = 56230;
				taid = 1;
				tidx1 = 4;
				widx0 = 7;
				apptype = 0;
				eitype = 6;
				tidx0 = 2;
				ectype = -1;
				it2 = 56232;
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
			cont = 56208;
			gtype = 3;
			oklnk = 2;
			area = 56201;
			goal = 56204;
			sort = 56205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 56202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 56207;
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
				minlvl = 20;
			}
			CDboTSCheckClrQst
			{
				and = "544;";
				flink = 1;
				flinknextqid = "568;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3142109;";
			}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 56214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 56201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142109;";
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
				conv = 56209;
				ctype = 1;
				idx = 3142109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 56208;
				gtype = 3;
				area = 56201;
				goal = 56204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 56205;
				stype = 1;
				taid = 1;
				title = 56202;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

