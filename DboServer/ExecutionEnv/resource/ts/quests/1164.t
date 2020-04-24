CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1164;
	title = 116402;

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
				conv = 116409;
				ctype = 1;
				idx = 4192103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 116408;
				gtype = 1;
				area = 116401;
				goal = 116404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 116405;
				stype = 1;
				taid = 1;
				title = 116402;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 116408;
			gtype = 1;
			oklnk = 2;
			area = 116401;
			goal = 116404;
			sort = 116405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 116402;
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
			desc = 116414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 116401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242111;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 116407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSClickNPC
			{
				npcidx = "4192103;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1165;";
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
	}
}

