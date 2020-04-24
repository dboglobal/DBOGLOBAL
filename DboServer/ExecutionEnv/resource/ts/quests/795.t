CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 795;
	title = 79502;

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
				conv = 79509;
				ctype = 1;
				idx = 3173201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 79508;
				gtype = 1;
				area = 79501;
				goal = 79504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 79505;
				stype = 2;
				taid = 1;
				title = 79502;
			}
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
			desc = 79514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 79501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031207;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 79508;
			gtype = 1;
			oklnk = 2;
			area = 79501;
			goal = 79504;
			sort = 79505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 79502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 79507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 27;
			}
			CDboTSCheckClrQst
			{
				and = "794;";
				flink = 1;
				flinknextqid = "796;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3173201;";
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
	}
}

