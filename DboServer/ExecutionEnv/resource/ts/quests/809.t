CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 809;
	title = 80902;

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
				conv = 80909;
				ctype = 1;
				idx = 3181203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 80908;
				gtype = 1;
				area = 80901;
				goal = 80904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 80905;
				stype = 1;
				taid = 1;
				title = 80902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 80907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 27;
			}
			CDboTSClickNPC
			{
				npcidx = "3181203;";
			}
			CDboTSCheckClrQst
			{
				and = "808;";
				flink = 1;
				flinknextqid = "810;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 80908;
			gtype = 1;
			oklnk = 2;
			area = 80901;
			goal = 80904;
			sort = 80905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 80902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 80914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 80901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1111102;";
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

