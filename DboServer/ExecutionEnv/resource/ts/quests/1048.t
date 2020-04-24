CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1048;
	title = 104802;

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
				conv = 104809;
				ctype = 1;
				idx = 5063107;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 6312103;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 104821;
				taid = 3;
				widx0 = 5;
				dt0 = 104822;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 104808;
				gtype = 2;
				area = 104801;
				goal = 104804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 104805;
				stype = 2;
				taid = 1;
				title = 104802;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 104823;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 104820;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

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
			cont = 104808;
			gtype = 2;
			oklnk = 2;
			area = 104801;
			goal = 104804;
			sort = 104805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 104802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 104814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 104801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063107;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 104807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 37;
			}
			CDboTSCheckClrQst
			{
				and = "1047;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5063107;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
	}
}

