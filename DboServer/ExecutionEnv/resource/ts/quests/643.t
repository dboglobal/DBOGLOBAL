CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 643;
	title = 64302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 64307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "1311208;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "644;";
				not = 0;
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
				conv = 64309;
				ctype = 1;
				idx = 1311208;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 64308;
				gtype = 1;
				area = 64301;
				goal = 64304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 64305;
				stype = 1;
				taid = 1;
				title = 64302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 64308;
			gtype = 1;
			oklnk = 2;
			area = 64301;
			goal = 64304;
			sort = 64305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 64302;
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
			desc = 64314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 64301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6311202;";
			}
		}
	}
}

