CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2437;
	title = 243702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2437;
				start = 0;
				taid = 1;
				uof = 0;
			}
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
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 243709;
				ctype = 1;
				idx = 3291202;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2437;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 243708;
				gtype = 3;
				area = 243701;
				goal = 243704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 243705;
				stype = 2;
				taid = 1;
				title = 243702;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 547;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 243708;
			gtype = 3;
			oklnk = 2;
			area = 243701;
			goal = 243704;
			sort = 243705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 243702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 243714;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 243701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5531501;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 243707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSCheckClrQst
			{
				and = "2436;";
				flink = 1;
				flinknextqid = "2438;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3291202;";
			}
		}
	}
}

