CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3735;
	title = 373502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 373501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1752105;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 373509;
				ctype = 1;
				idx = 1751110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 373508;
				gtype = 1;
				area = 373501;
				goal = 373504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 373505;
				stype = 2;
				taid = 1;
				title = 373502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 373508;
			gtype = 1;
			oklnk = 2;
			area = 373501;
			goal = 373504;
			sort = 373505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 373502;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSCheckClrQst
			{
				and = "3734;";
				flink = 0;
				flinknextqid = "3736;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1751110;";
			}
		}
	}
}

