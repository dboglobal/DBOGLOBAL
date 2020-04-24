CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1351;
	title = 135102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 135109;
				ctype = 1;
				idx = 5313201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 135108;
				gtype = 1;
				area = 135101;
				goal = 135104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 135105;
				stype = 2;
				taid = 1;
				title = 135102;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			desc = 135114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 135101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131122;";
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
			cont = 135108;
			gtype = 1;
			oklnk = 2;
			area = 135101;
			goal = 135104;
			sort = 135105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 135102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 135107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSCheckClrQst
			{
				and = "1350;";
				flink = 1;
				flinknextqid = "1356;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5313201;";
			}
		}
	}
}

