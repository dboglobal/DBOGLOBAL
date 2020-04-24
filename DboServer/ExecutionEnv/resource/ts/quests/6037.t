CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6037;
	title = 603702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActSendSvrEvt
			{
				id = 428;
				tblidx = 3174103;
				stype = 4;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 603709;
				ctype = 1;
				idx = 4751108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 603708;
				area = 603701;
				goal = 603704;
				m0fz = "3947.220947";
				m0widx = 1;
				scitem = -1;
				sort = 603705;
				stype = 8;
				m0fy = "-88.579002";
				m0pat = 2;
				grade = -1;
				m0fx = "5731.930176";
				m0ttip = 603715;
				rwd = 100;
				taid = 1;
				title = 603702;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 253;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 6037;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 254;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 6037;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSSkillUse
			{
				idx = 420111;
				rp = 0;
				stype = 6;
				tidx = 3174103;
				ttype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 603714;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 603701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 603708;
			gtype = 1;
			oklnk = 2;
			area = 603701;
			goal = 603704;
			sort = 603705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 603702;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "5;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 603707;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4751108;";
			}
			CDboTSCheckClrQst
			{
				and = "6036;";
				flink = 0;
				not = 0;
			}
		}
	}
}

