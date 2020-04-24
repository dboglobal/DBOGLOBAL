CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2603;
	title = 260302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 260309;
				ctype = 1;
				idx = 1657102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 5411102;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 260320;
				taid = 3;
				widx0 = 1;
				dt0 = 260321;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 260308;
				gtype = 2;
				area = 260301;
				goal = 260304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 260305;
				stype = 2;
				taid = 1;
				title = 260302;
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
			cont = 260308;
			gtype = 2;
			oklnk = 2;
			area = 260301;
			goal = 260304;
			sort = 260305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 260302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 260307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1657102;";
			}
			CDboTSCheckClrQst
			{
				and = "2602;";
				flink = 1;
				flinknextqid = "2604;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 260314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 260301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1657102;";
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

