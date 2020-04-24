CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1592;
	title = 159202;

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
				dt2 = 159225;
				otype2 = 7;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 7;
				widx2 = 1;
				dt0 = 159221;
				esctype = 0;
				iidx1 = -1;
				it1 = 159222;
				otype0 = 7;
				tidx2 = 719;
				dt1 = 159223;
				etype = 0;
				iidx2 = -1;
				it0 = 159220;
				taid = 1;
				tidx1 = 718;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 717;
				ectype = -1;
				it2 = 159224;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 159209;
				ctype = 1;
				idx = 5063106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 159208;
				gtype = 2;
				area = 159201;
				goal = 159204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 159205;
				stype = 2;
				taid = 1;
				title = 159202;
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
			cont = 159208;
			gtype = 2;
			oklnk = 2;
			area = 159201;
			goal = 159204;
			sort = 159205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 159202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 159214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 159201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 159207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSCheckClrQst
			{
				and = "1591;";
				flink = 1;
				flinknextqid = "1597;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5063106;";
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
	}
}

