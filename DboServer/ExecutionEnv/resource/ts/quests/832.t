CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 832;
	title = 83202;

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
				conv = 83209;
				ctype = 1;
				idx = 3031401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 83208;
				gtype = 1;
				area = 83201;
				goal = 83204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 83205;
				stype = 2;
				taid = 1;
				title = 83202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 83214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 83201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311201;";
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
			cont = 83208;
			gtype = 1;
			oklnk = 2;
			area = 83201;
			goal = 83204;
			sort = 83205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 83202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 83207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "3031401;";
			}
			CDboTSCheckClrQst
			{
				and = "831;";
				flink = 1;
				flinknextqid = "833;";
				not = 0;
			}
		}
	}
}

