CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 361;
	title = 36102;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 36108;
			gtype = 2;
			oklnk = 2;
			area = 36101;
			goal = 36104;
			sort = 36105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 36102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 36107;
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
				minlvl = 20;
			}
			CDboTSCheckClrQst
			{
				and = "359;";
				flink = 1;
				flinknextqid = "362;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3172203;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 36114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 36101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172203;";
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
				apptype = 0;
				eitype = 6;
				tidx0 = 4651206;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 36120;
				taid = 1;
				widx0 = 1;
				dt0 = 36121;
				esctype = 0;
				otype0 = 1;
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
				conv = 36109;
				ctype = 1;
				idx = 3172203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 36108;
				gtype = 2;
				area = 36101;
				goal = 36104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 36105;
				stype = 1;
				taid = 1;
				title = 36102;
			}
		}
	}
}

