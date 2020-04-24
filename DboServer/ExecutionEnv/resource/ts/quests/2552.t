CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2552;
	title = 255202;

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
				conv = 255209;
				ctype = 1;
				idx = 5537101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 255208;
				gtype = 1;
				area = 255201;
				goal = 255204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 255205;
				stype = 1;
				taid = 1;
				title = 255202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255207;
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
				npcidx = "5537101;";
			}
			CDboTSCheckClrQst
			{
				and = "2551;";
				flink = 1;
				flinknextqid = "2553;";
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
			cont = 255208;
			gtype = 1;
			oklnk = 2;
			area = 255201;
			goal = 255204;
			sort = 255205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 255202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 255214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 255201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189105;";
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

