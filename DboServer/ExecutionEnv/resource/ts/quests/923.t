CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 923;
	title = 92302;

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
				apptype = 0;
				eitype = 6;
				tidx0 = 3142101;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 92320;
				taid = 1;
				widx0 = 1;
				dt0 = 92321;
				esctype = 0;
				otype0 = 1;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 92307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 32;
			}
			CDboTSClickNPC
			{
				npcidx = "3142110;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "924;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
				conv = 92309;
				ctype = 1;
				idx = 3142110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 92308;
				gtype = 1;
				area = 92301;
				goal = 92304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 92305;
				stype = 2;
				taid = 1;
				title = 92302;
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
			cont = 92308;
			gtype = 1;
			oklnk = 2;
			area = 92301;
			goal = 92304;
			sort = 92305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 92302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 92314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 92301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142110;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

