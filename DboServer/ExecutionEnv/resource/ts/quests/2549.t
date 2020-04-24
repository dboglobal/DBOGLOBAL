CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2549;
	title = 254902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 254908;
				gtype = 2;
				area = 254901;
				goal = 254904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 254905;
				stype = 1;
				taid = 1;
				title = 254902;
			}
			CDboTSActSToCEvt
			{
				dt2 = 254925;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 254921;
				esctype = 0;
				iidx1 = -1;
				it1 = 254922;
				otype0 = 1;
				tidx2 = 3182107;
				dt1 = 254923;
				etype = 0;
				iidx2 = -1;
				it0 = 254920;
				taid = 2;
				tidx1 = 4531101;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3183115;
				ectype = -1;
				it2 = 254924;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 254908;
			gtype = 2;
			oklnk = 2;
			area = 254901;
			goal = 254904;
			sort = 254905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 254902;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 254914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 254901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189105;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 254907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickObject
			{
				objidx = "1454;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2550;";
				not = 0;
			}
		}
	}
}

