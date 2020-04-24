CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 702;
	title = 70202;

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
				conv = 70209;
				ctype = 1;
				idx = 5061202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 70208;
				gtype = 1;
				area = 70201;
				goal = 70204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 70205;
				stype = 2;
				taid = 1;
				title = 70202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 70207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "5061202;";
			}
			CDboTSCheckClrQst
			{
				and = "701;";
				flink = 1;
				flinknextqid = "703;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 70208;
			gtype = 1;
			oklnk = 2;
			area = 70201;
			goal = 70204;
			sort = 70205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 70202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 70214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 70201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151105;";
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
	}
}

